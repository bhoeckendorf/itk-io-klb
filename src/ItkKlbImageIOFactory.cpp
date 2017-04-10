#include "ItkKlbImageIOFactory.h"
#include "ItkKlbImageIO.h"
#include "itkVersion.h"


ItkKlbImageIOFactory::ItkKlbImageIOFactory()
{
    this->RegisterOverride("itkImageIOBase", "ItkKlbImageIO", "KLB Image IO", 1, itk::CreateObjectFunction< ItkKlbImageIO >::New());
}


ItkKlbImageIOFactory::~ItkKlbImageIOFactory()
{}


const char* ItkKlbImageIOFactory::GetITKSourceVersion(void) const
{
    return ITK_SOURCE_VERSION;
}


const char* ItkKlbImageIOFactory::GetDescription() const
{
    return "KLB ImageIO Factory, allows the loading of Keller Lab Block File images into insight";
}
