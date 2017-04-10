#ifndef ITKKLBIMAGEIO_H
#define ITKKLBIMAGEIO_H

#include "itkImage.h"
#include "itkImageIOBase.h"
class klb_imageIO;

#if defined(COMPILE_SHARED_LIBRARY) && defined(_MSC_VER)
class __declspec(dllexport) ItkKlbImageIO
#else
class ItkKlbImageIO
#endif
    : public itk::ImageIOBase
{
public:
    typedef ItkKlbImageIO                  Self;
    typedef itk::ImageIOBase               Superclass;
    typedef itk::SmartPointer< Self >      Pointer;
    typedef itk::SmartPointer< const Self> ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(KlbItkImageIO, itk::ImageIOBase);

    virtual bool SupportsDimension(unsigned long) ITK_OVERRIDE;

    virtual bool CanReadFile(const char*) ITK_OVERRIDE;
    virtual void ReadImageInformation() ITK_OVERRIDE;
    virtual void Read(void* buffer) ITK_OVERRIDE;

    virtual bool CanWriteFile(const char*) ITK_OVERRIDE;
    virtual void WriteImageInformation() ITK_OVERRIDE;
    virtual void Write(const void* buffer) ITK_OVERRIDE;

    int getNumThreads();
    void setNumThreads(const int n);

protected:
    ItkKlbImageIO();
    ~ItkKlbImageIO();
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
    inline itk::ImageIOBase::IOComponentType KlbToItkType(const int klbType) const;
    inline int ItkToKlbType(const itk::ImageIOBase::IOComponentType itkType) const;

    klb_imageIO* m_io;
    int m_numThreads;

    ITK_DISALLOW_COPY_AND_ASSIGN(ItkKlbImageIO);
};

#endif // ITKKLBIMAGEIO_H
