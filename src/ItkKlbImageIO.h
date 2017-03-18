#ifndef ItkKlbImageIO_h
#define ItkKlbImageIO_h

#include "itkImage.h"
#include "itkImageIOBase.h"
class klb_image_header;

class ItkKlbImageIO: public itk::ImageIOBase
{
public:
    typedef ItkKlbImageIO             Self;
    typedef itk::ImageIOBase          Superclass;
    typedef itk::SmartPointer< Self > Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(KlbItkImageIO, itk::ImageIOBase);

    /** The different types of ImageIO's can support data of varying
    * dimensionality. For example, some file formats are strictly 2D
    * while others can support 2D, 3D, or even n-D. This method returns
    * true/false as to whether the ImageIO can support the dimension
    * indicated. */
    virtual bool SupportsDimension(unsigned long) ITK_OVERRIDE;

    /** Determine the file type. Returns true if this ImageIO can read the
    * file specified. */
    virtual bool CanReadFile(const char*) ITK_OVERRIDE;

    /** Set the spacing and dimension information for the set filename. */
    virtual void ReadImageInformation() ITK_OVERRIDE;

    /** Reads the data from disk into the memory buffer provided. */
    virtual void Read(void* buffer) ITK_OVERRIDE;

    /** Determine the file type. Returns true if this ImageIO can write the
    * file specified. */
    virtual bool CanWriteFile(const char*) ITK_OVERRIDE;

    /** Set the spacing and dimension information for the set filename. */
    virtual void WriteImageInformation() ITK_OVERRIDE;

    /** Writes the data to disk from the memory buffer provided. Make sure
    * that the IORegions has been set properly. */
    virtual void Write(const void* buffer) ITK_OVERRIDE;

	int getNumThreads();
	void setNumThreads(const int n);

protected:
    ItkKlbImageIO();
    ~ItkKlbImageIO();
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
    klb_image_header* m_header;
    int m_numThreads;
};

#endif // ItkKlbImageIO_h
