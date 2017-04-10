#ifndef ITKKLBIMAGEIOFACROTY_H
#define ITKKLBIMAGEIOFACROTY_H
#include "ITKIONRRDExport.h"

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

#if defined(COMPILE_SHARED_LIBRARY) && defined(_MSC_VER)
class __declspec(dllexport) ItkKlbImageIOFactory
#else
class ItkKlbImageIOFactory
#endif
    : public itk::ObjectFactoryBase
{
public:
    typedef ItkKlbImageIOFactory            Self;
    typedef ObjectFactoryBase               Superclass;
    typedef itk::SmartPointer< Self >       Pointer;
    typedef itk::SmartPointer< const Self > ConstPointer;

    virtual const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
    virtual const char* GetDescription(void) const ITK_OVERRIDE;

    itkFactorylessNewMacro(Self);
    itkTypeMacro(ItkKlbImageIOFactory, itk::ObjectFactoryBase);

    static void RegisterOneFactory(void)
    {
        ItkKlbImageIOFactory::Pointer factory = ItkKlbImageIOFactory::New();
        itk::ObjectFactoryBase::RegisterFactoryInternal(factory);
    }

protected:
    ItkKlbImageIOFactory();
    ~ItkKlbImageIOFactory();

private:
    ITK_DISALLOW_COPY_AND_ASSIGN(ItkKlbImageIOFactory);
};

#endif // ITKKLBIMAGEIOFACROTY_H
