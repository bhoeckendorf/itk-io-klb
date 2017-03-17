#ifndef ITKKLBIMAGEIOTEST_H
#define ITKKLBIMAGEIOTEST_H

#include "gtest/gtest.h"
#include <cstdio>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "ItkKlbImageIO.h"


TEST(ItkKlbImageIO, write)
{
    typedef unsigned short PixelType;
    const unsigned char Dims = 3;

    typedef itk::Image<PixelType, Dims> ImageType;
    typedef itk::ImageFileWriter<ImageType> WriterType;

    ImageType::RegionType region;
    ImageType::IndexType origin;
    ImageType::SizeType size;
    for (int i = 0; i < Dims; ++i)
    {
        origin[i] = 0;
        size[i] = 64;
    }
    region.SetSize(size);
    region.SetIndex(origin);
    
    ImageType::Pointer img = ImageType::New();
    img->SetRegions(region);
    img->Allocate();
    
    ImageType::IndexType idx;
    for (int i = 0; i < Dims; ++i)
        idx[i] = 0;

    for (int i = 0; i < 10; ++i)
    {
        idx[0] = i;
        img->SetPixel(idx, (PixelType) i);
    }

    WriterType::Pointer writer = WriterType::New();
    writer->SetImageIO(ItkKlbImageIO::New());
    writer->SetFileName("_deleteme_ItkKlbUnitTest.klb");
    writer->SetInput(img);
    writer->Update();
}


TEST(ItkKlbImageIO, read)
{
    const std::string fname("_deleteme_ItkKlbUnitTest.klb");

    typedef unsigned short PixelType;
    const unsigned char Dims = 3;

    typedef itk::Image<PixelType, Dims> ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetImageIO(ItkKlbImageIO::New());
    reader->SetFileName(fname);
    reader->Update();
    ImageType::Pointer img = reader->GetOutput();

    ImageType::IndexType idx;
    for (int i = 0; i < Dims; ++i)
        idx[i] = 0;
    
    for (int i = 0; i < 10; ++i)
    {
        idx[0] = i;
        EXPECT_EQ(img->GetPixel(idx), (PixelType) i);
    }

    if (remove(fname.c_str()) != 0)
        std::cerr << "Warning: Failed to remove file " << fname << " created during unit tests." << std::endl;
}

#endif // ITKKLBIMAGEIOTEST_H
