#include "ItkKlbImageIO.h"
#include <iostream>
#include <regex>
#include <string>
#include <thread>

// KLB imports
#include "common.h"
#include "klb_imageHeader.h"
#include "klb_imageIO.h"


ItkKlbImageIO::ItkKlbImageIO()
{
    AddSupportedReadExtension(".klb");
    AddSupportedWriteExtension(".klb");
    m_numThreads = (int)std::thread::hardware_concurrency();
    m_io = new klb_imageIO();
}


ItkKlbImageIO::~ItkKlbImageIO()
{
    delete m_io;
}


itk::ThreadIdType ItkKlbImageIO::GetNumberOfThreads()
{
    return (itk::ThreadIdType)m_numThreads;
}


void ItkKlbImageIO::SetNumberOfThreads(itk::ThreadIdType n)
{
    m_numThreads = (int)n;
}


bool ItkKlbImageIO::SupportsDimension(unsigned long dim)
{
    return dim < KLB_DATA_DIMS;
}


bool ItkKlbImageIO::CanReadFile(const char* filename)
{
    std::string fname = filename;
    std::string::size_type pos = fname.rfind(".klb");
    return pos != std::string::npos && pos == fname.length() - 4;
}


void ItkKlbImageIO::ReadImageInformation()
{
    m_io->setFilename(GetFileName());
    const int err = m_io->readHeader();

    SetComponentType(KlbToItkType(m_io->header.dataType));

    int lastNonSingletonDimension = KLB_DATA_DIMS - 1;
    while (m_io->header.xyzct[lastNonSingletonDimension] == 1)
    {
        lastNonSingletonDimension--;
    }
    SetNumberOfDimensions(lastNonSingletonDimension + 1);
    for (int i = 0; i <= lastNonSingletonDimension; ++i)
    {
        SetDimensions(i, m_io->header.xyzct[i]);
        SetSpacing(i, m_io->header.pixelSize[i]);
    }
}


void ItkKlbImageIO::Read(void* buffer)
{
    m_io->readImageFull((char*)buffer, m_numThreads);
}


void ItkKlbImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
    Superclass::PrintSelf(os, indent);
}


bool ItkKlbImageIO::CanWriteFile(const char* name)
{
    return CanReadFile(name);
}


void ItkKlbImageIO::WriteImageInformation(void)
{}


void ItkKlbImageIO::Write(const void* buffer)
{
    m_io->setFilename(GetFileName());
    m_io->header.dataType = (KLB_DATA_TYPE)ItkToKlbType(GetComponentType());
    for (unsigned int i = 0; i < GetNumberOfDimensions(); ++i)
    {
        m_io->header.xyzct[i] = (uint32_t)GetDimensions(i);
        m_io->header.pixelSize[i] = (float32_t)GetSpacing(i);
    }
    for (int i = GetNumberOfDimensions(); i < KLB_DATA_DIMS; ++i)
    {
        m_io->header.xyzct[i] = (uint32_t)1;
        m_io->header.pixelSize[i] = (float32_t)1;
    }
    m_io->writeImage((const char*)buffer, m_numThreads);
}


inline itk::ImageIOBase::IOComponentType ItkKlbImageIO::KlbToItkType(const int klbType) const
{
    switch (klbType)
    {
    case KLB_DATA_TYPE::UINT8_TYPE:
        return UCHAR;
    case KLB_DATA_TYPE::UINT16_TYPE:
        return USHORT;
    case KLB_DATA_TYPE::UINT32_TYPE:
        return UINT;
    case KLB_DATA_TYPE::UINT64_TYPE:
        return ULONG;

    case KLB_DATA_TYPE::INT8_TYPE:
        return CHAR;
    case KLB_DATA_TYPE::INT16_TYPE:
        return SHORT;
    case KLB_DATA_TYPE::INT32_TYPE:
        return INT;
    case KLB_DATA_TYPE::INT64_TYPE:
        return LONG;

    case KLB_DATA_TYPE::FLOAT32_TYPE:
        return FLOAT;
    case KLB_DATA_TYPE::FLOAT64_TYPE:
        return DOUBLE;

    default:
        return UNKNOWNCOMPONENTTYPE;
    }
}


inline int ItkKlbImageIO::ItkToKlbType(const itk::ImageIOBase::IOComponentType itkType) const
{
    switch (itkType)
    {
    case UCHAR:
        return KLB_DATA_TYPE::UINT8_TYPE;
    case USHORT:
        return KLB_DATA_TYPE::UINT16_TYPE;
    case UINT:
        return KLB_DATA_TYPE::UINT32_TYPE;
    case ULONG:
        return KLB_DATA_TYPE::UINT64_TYPE;

    case CHAR:
        return KLB_DATA_TYPE::INT8_TYPE;
    case SHORT:
        return KLB_DATA_TYPE::INT16_TYPE;
    case INT:
        return KLB_DATA_TYPE::INT32_TYPE;
    case LONG:
        return KLB_DATA_TYPE::INT64_TYPE;

    case FLOAT:
        return KLB_DATA_TYPE::FLOAT32_TYPE;
    case DOUBLE:
        return KLB_DATA_TYPE::FLOAT64_TYPE;

    default:
        std::cerr << "Error: Unknown or unsupported data type." << std::endl;
        exit(EXIT_FAILURE);
    }
}
