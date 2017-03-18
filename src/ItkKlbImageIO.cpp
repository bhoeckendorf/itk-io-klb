#include "ItkKlbImageIO.h"

#include <fstream>
#include <string>
#include <regex>
#include <thread>

#include "klb_imageHeader.h"
#include "klb_Cwrapper.h"
#include "common.h"

ItkKlbImageIO::ItkKlbImageIO()
{
    AddSupportedReadExtension( ".klb" );
    AddSupportedWriteExtension( ".klb" );
	m_numThreads = std::thread::hardware_concurrency();
    m_header = new klb_image_header();
}

ItkKlbImageIO::~ItkKlbImageIO()
{
    delete m_header;
}

int ItkKlbImageIO::getNumThreads()
{
	return m_numThreads;
}

void ItkKlbImageIO::setNumThreads(const int n)
{
	m_numThreads = n;
}

bool ItkKlbImageIO::SupportsDimension( unsigned long dim )
{
    return dim < 5;
}

bool ItkKlbImageIO::CanReadFile( const char* filename )
{
    std::string fname = filename;
    std::string::size_type pos = fname.rfind( ".klb" );
    return pos != std::string::npos && pos == fname.length() - 4;
}

void ItkKlbImageIO::ReadImageInformation()
{
    const int err = m_header->readHeader( GetFileName() );

    switch ( m_header->dataType )
    {
        case KLB_DATA_TYPE::UINT8_TYPE:
            SetComponentType( UCHAR );
            break;
        case KLB_DATA_TYPE::UINT16_TYPE:
            SetComponentType( USHORT );
            break;
        case KLB_DATA_TYPE::UINT32_TYPE:
            SetComponentType( UINT );
            break;
        case KLB_DATA_TYPE::UINT64_TYPE:
            SetComponentType( ULONG );
            break;

        case KLB_DATA_TYPE::INT8_TYPE:
            SetComponentType( CHAR );
            break;
        case KLB_DATA_TYPE::INT16_TYPE:
            SetComponentType( SHORT );
            break;
        case KLB_DATA_TYPE::INT32_TYPE:
            SetComponentType( INT );
            break;
        case KLB_DATA_TYPE::INT64_TYPE:
            SetComponentType( LONG );
            break;

        case KLB_DATA_TYPE::FLOAT32_TYPE:
            SetComponentType( FLOAT );
            break;
        case KLB_DATA_TYPE::FLOAT64_TYPE:
            SetComponentType( DOUBLE );
            break;

        default:
            SetComponentType( UNKNOWNCOMPONENTTYPE );
    }

    int lastNonSingletonDimension = 4;
    while ( m_header->xyzct[lastNonSingletonDimension] == 1 )
    {
        lastNonSingletonDimension--;
    }
    SetNumberOfDimensions( lastNonSingletonDimension + 1 );
    for (int i = 0; i <= lastNonSingletonDimension; ++i)
    {
        SetDimensions( i, m_header->xyzct[i] );
		SetSpacing( i, m_header->pixelSize[i] );
    }
}

void ItkKlbImageIO::Read( void* buffer )
{
    KLB_DATA_TYPE dtype;
    readKLBstackInPlace( GetFileName(), buffer, &dtype, m_numThreads );
}

void ItkKlbImageIO::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
    Superclass::PrintSelf( os, indent );
}

bool ItkKlbImageIO::CanWriteFile( const char* name ) { return CanReadFile( name ); }

void ItkKlbImageIO::WriteImageInformation( void ) {}

void ItkKlbImageIO::Write( const void* buffer )
{
    unsigned int imgsize[5];
	float sampling[5];
    unsigned long imgBytes = GetComponentSize();
    for (unsigned int i = 0; i < GetNumberOfDimensions(); ++i)
    {
        const SizeValueType v = GetDimensions( i );
        imgsize[ i ] = v;
        imgBytes *= v;
		sampling[ i ] = GetSpacing( i );
    }
    for (int i = GetNumberOfDimensions(); i < 5; ++i)
    {
        imgsize[ i ] = 1;
    }

    KLB_DATA_TYPE dtype;
    switch ( GetComponentType() )
    {
        case UCHAR:
            dtype = KLB_DATA_TYPE::UINT8_TYPE;
            break;
        case USHORT:
            dtype = KLB_DATA_TYPE::UINT16_TYPE;
            break;
        case UINT:
            dtype = KLB_DATA_TYPE::UINT32_TYPE;
            break;
        case ULONG:
            dtype = KLB_DATA_TYPE::UINT64_TYPE;
            break;

        case CHAR:
            dtype = KLB_DATA_TYPE::INT8_TYPE;
            break;
        case SHORT:
            dtype = KLB_DATA_TYPE::INT16_TYPE;
            break;
        case INT:
            dtype = KLB_DATA_TYPE::INT32_TYPE;
            break;
        case LONG:
            dtype = KLB_DATA_TYPE::INT64_TYPE;
            break;

        case FLOAT:
            dtype = KLB_DATA_TYPE::FLOAT32_TYPE;
            break;
        case DOUBLE:
            dtype = KLB_DATA_TYPE::FLOAT64_TYPE;
    }

    writeKLBstack( buffer, GetFileName(), imgsize, dtype, m_numThreads, sampling, NULL, KLB_COMPRESSION_TYPE::BZIP2, NULL);
}
