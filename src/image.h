#ifndef IMAGE_H
#define IMAGE_H

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>
#include <itkNiftiImageIO.h>

#include "armadillo"

#include <QDebug>


enum Axis { X, Y, Z, NONE };

struct surface {
    arma::fmat vertices;
    arma::umat faces;
    arma::fmat colors;
    arma::fmat normals;
};

typedef float                       PixelType;
typedef itk::Image< PixelType, 2 >  Image2DType;
typedef itk::Image< PixelType, 3 >  Image3DType;
typedef itk::Image< PixelType, 4 >  Image4DType;

typedef itk::NiftiImageIO           NiftiType;

typedef Image2DType::IndexType      Index2DType;
typedef Image2DType::SizeType       Size2DType;
typedef Image2DType::RegionType     Region2DType;
typedef Image2DType::SpacingType    Spacing2DType;
typedef Image2DType::PointType      Origin2DType;
typedef Image2DType::DirectionType  Direction2DType;

typedef Image3DType::IndexType      Index3DType;
typedef Image3DType::SizeType       Size3DType;
typedef Image3DType::RegionType     Region3DType;
typedef Image3DType::SpacingType    Spacing3DType;
typedef Image3DType::PointType      Origin3DType;
typedef Image3DType::DirectionType  Direction3DType;

typedef Image4DType::IndexType      Index4DType;
typedef Image4DType::SizeType       Size4DType;
typedef Image4DType::SpacingType    Spacing4DType;
typedef Image4DType::PointType      Origin4DType;
typedef Image4DType::DirectionType  Direction4DType;


inline itk::ImageIOBase::Pointer getImageIO(std::string input){
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(input.c_str(), itk::ImageIOFactory::ReadMode);

    imageIO->UseCompressionOn();
    imageIO->SetUseStreamedReading(true);
    imageIO->SetFileName(input);
    imageIO->ReadImageInformation();

    return imageIO;
}

inline itk::ImageIOBase::IOComponentType component_type(itk::ImageIOBase::Pointer imageIO){
    return imageIO->GetComponentType();
}

inline itk::ImageIOBase::IOPixelType pixel_type(itk::ImageIOBase::Pointer imageIO){
    return imageIO->GetPixelType();
}

inline size_t num_dimensions(itk::ImageIOBase::Pointer imageIO){
    return imageIO->GetNumberOfDimensions();
}

template<typename TImageType>
static void ReadImage(std::string filename, typename TImageType::Pointer image)
{
    itk::ImageIOBase::Pointer imageIO = getImageIO(filename);

    // In order to read a image, we need its dimensionality and component type
    std::cout << "numDimensions: " << num_dimensions(imageIO) << std::endl;
    std::cout << "component type: " << imageIO->GetComponentTypeAsString(component_type(imageIO)) << std::endl;
    // The pixel type is not necessary. This is just to let you know that it exists
    std::cout << "pixel type: " << imageIO->GetPixelTypeAsString(pixel_type(imageIO)) << std::endl;

    typedef itk::ImageFileReader<TImageType> ReaderType;
    typename ReaderType::Pointer reader = ReaderType::New();

    reader->SetNumberOfThreads(5);
    reader->SetImageIO(imageIO);
    reader->SetUseStreaming(true);
    reader->SetFileName(filename);

    try {
        reader->Update();
    } catch (itk::ExceptionObject &err) {
        qDebug() << "ExceptionObject caught !";
        std::cerr <<  err << std::endl;
        std::cerr << "Error reading file " << filename << std::endl;

    }
    image->Graft(reader->GetOutput());
}


#endif // IMAGE_H
