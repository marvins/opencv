/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include "grfmt_gdal.hpp"

/// C++ Standard Libraries
#include <iostream>
#include <stdexcept>
#include <string>

namespace cv{


/**
 * Convert gdal type to opencv type
*/
int gdal2opencv( const GDALDataType& gdalType, const int& channels ){
    
    switch( gdalType ){

        /// UInt8
        case GDT_Byte:
            if( channels == 1 ){ return CV_8UC1; }
            if( channels == 3 ){ return CV_8UC3; }
            if( channels == 4 ){ return CV_8UC4; }
            return -1;
        
        /// UInt16
        case GDT_UInt16:
            if( channels == 1 ){ return CV_16UC1; }
            if( channels == 3 ){ return CV_16UC3; }
            if( channels == 4 ){ return CV_16UC4; }
            return -1;

        /// Int16
        case GDT_Int16:
            if( channels == 1 ){ return CV_16SC1; }
            if( channels == 3 ){ return CV_16SC3; }
            if( channels == 4 ){ return CV_16SC4; }
            return -1;

        /// UInt32
        case GDT_UInt32:
        case GDT_Int32:
            if( channels == 1 ){ return CV_32SC1; }
            if( channels == 3 ){ return CV_32SC3; }
            if( channels == 4 ){ return CV_32SC4; }
            return -1;

        default:
            std::cout << "Unknown GDAL Data Type" << std::endl;
            std::cout << "Type: " << GDALGetDataTypeName(gdalType) << std::endl;
            return -1;
    }

    return -1;
}


std::string GetOpenCVTypeName( const int& type ){

    switch(type){
        case CV_8UC1:
            return "CV_8UC1";
        case CV_8UC3:
            return "CV_8UC3";
        case CV_8UC4:
            return "CV_8UC4";
        case CV_16UC1:
            return "CV_16UC1";
        case CV_16UC3:
            return "CV_16UC3";
        case CV_16UC4:
            return "CV_16UC4";
        case CV_16SC1:
            return "CV_16SC1";
        case CV_16SC3:
            return "CV_16SC3";
        case CV_16SC4:
            return "CV_16SC4";
        default:
            return "Unknown";
    }
    return "Unknown";
}


/**
 * GDAL Decoder Constructor
*/
GdalDecoder::GdalDecoder(){

    std::cout << "gdal decoder constructor" << std::endl;
    
    // set a dummy signature
    m_signature="000000000000";
    
    /// Register the driver
    GDALAllRegister();

    m_driver = NULL;
    m_dataset = NULL;
}

/**
 * GDAL Decoder Destructor
*/
GdalDecoder::~GdalDecoder(){

    std::cout << "gdal decoder destructor" << std::endl;
    
    if( m_dataset != NULL ){
       close(); 
    }
}

/**
 * Convert data range
*/
double range_cast( const GDALDataType& gdalType, const int& cvDepth, const double& value ){

    // uint8 -> uint8
    if( gdalType == GDT_Byte && cvDepth == CV_8U ){
        return value;
    }
    // uint8 -> uint16
    if( gdalType == GDT_Byte && (cvDepth == CV_16U || cvDepth == CV_16S)){
        return (value*256);
    }

    // uint8 -> uint32
    if( gdalType == GDT_Byte && (cvDepth == CV_32F || cvDepth == CV_32S)){
        return (value*16777216);
    }
    
    // int16 -> uint8
    if( (gdalType == GDT_UInt16 || gdalType == GDT_Int16) && cvDepth == CV_8U ){
        return std::floor(value/256.0);
    }

    // int16 -> int16
    if( (gdalType == GDT_UInt16 || gdalType == GDT_Int16) && 
        ( cvDepth == CV_16U     ||  cvDepth == CV_16S   )){
        return value;
    }
    
    std::cout << GDALGetDataTypeName( gdalType ) << std::endl;
    std::cout << "warning: unknown range cast requested." << std::endl;
    return (value);
}

/**
 * There are some better mpl techniques for doing this.
*/
void write_pixel( const double& pixelValue, const GDALDataType& gdalType, const int& gdalChannels, Mat& image, const int& row, const int& col, const int& channel ){

    // convert the pixel
    double newValue = range_cast(gdalType, image.depth(), pixelValue );

    // input: 1 channel, output: 1 channel
    if( gdalChannels == 1 && image.channels() == 1 ){
        if( image.depth() == CV_8U ){       image.at<uchar>(row,col)          = newValue; }
        else if( image.depth() == CV_16U ){ image.at<unsigned short>(row,col) = newValue; }
        else if( image.depth() == CV_16S ){ image.at<short>(row,col)          = newValue; }
        else if( image.depth() == CV_32S ){ image.at<int>(row,col)            = newValue; }
        else if( image.depth() == CV_32F ){ image.at<float>(row,col)          = newValue; }
        else if( image.depth() == CV_64F ){ image.at<double>(row,col)         = newValue; }
        else{ throw std::runtime_error("Unknown image depth, gdal: 1, img: 1"); }
    }

    // input: 1 channel, output: 3 channel
    else if( gdalChannels == 1 && image.channels() == 3 ){
        if( image.depth() == CV_8U ){  image.at<Vec3b>(row,col) = Vec3b(newValue,newValue,newValue); }
        else{                          throw std::runtime_error("Unknown image depth, gdal:1, img: 3"); }
    }

    // input: 3 channel, output: 1 channel
    else if( gdalChannels == 3 && image.channels() == 1 ){
        if( image.depth() == CV_8U ){   image.at<uchar>(row,col) += (newValue/3.0); }
        else{ throw std::runtime_error("Unknown image depth, gdal:3, img: 1"); }
    }
    
    // input: 4 channel, output: 1 channel
    else if( gdalChannels == 4 && image.channels() == 1 ){
        if( image.depth() == CV_8U ){   image.at<uchar>(row,col) = newValue;  }
        else{ throw std::runtime_error("Unknown image depth, gdal: 4, image: 1"); }
    }
    
    // input: 3 channel, output: 3 channel
    else if( gdalChannels == 3 && image.channels() == 3 ){
        if( image.depth() == CV_8U ){  image.at<Vec3b>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_16U ){  image.at<Vec3s>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_16S ){  image.at<Vec3s>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_32S ){  image.at<Vec3i>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_32F ){  image.at<Vec3f>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_64F ){  image.at<Vec3d>(row,col)[channel] = newValue;  }
        else{ throw std::runtime_error("Unknown image depth, gdal: 3, image: 3"); }
    }

    // input: 4 channel, output: 3 channel
    else if( gdalChannels == 4 && image.channels() == 3 ){
        if( channel >= 4 ){ return; }
        else if( image.depth() == CV_8U  && channel < 4  ){  image.at<Vec3b>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_16U && channel < 4 ){  image.at<Vec3s>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_16S && channel < 4 ){  image.at<Vec3s>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_32S && channel < 4 ){  image.at<Vec3i>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_32F && channel < 4 ){  image.at<Vec3f>(row,col)[channel] = newValue;  }
        else if( image.depth() == CV_64F && channel < 4 ){  image.at<Vec3d>(row,col)[channel] = newValue;  }
        else{ throw std::runtime_error("Unknown image depth, gdal: 4, image: 3"); }
    }

    // input: 4 channel, output: 4 channel
    else if( gdalChannels == 4 && image.channels() == 4 ){
        if( image.depth() == CV_8U ){  image.at<Vec4b>(row,col)[channel] = newValue;  }
        else{ throw std::runtime_error("Unknown image depth, gdal: 4, image: 4"); }
    }

    // otherwise, throw an error
    else{
        throw std::runtime_error("error: can't convert types.");
    }

}

/**
 * read data
*/
bool GdalDecoder::readData( Mat& img ){

    std::cout << "gdal decoder readData" << std::endl;
    
    // make sure the image is the proper size
    if( img.size().height != m_height ){
        std::cout << "Input image has wrong height: " << img.size().height << " vs " << m_height << std::endl;
        return false;
    }
    if( img.size().width != m_width ){
        std::cout << "Input image has wrong width: " << img.size().width << " vs " << m_width << std::endl;
        return false;
    }

    // make sure the raster is alive
    if( m_dataset == NULL || m_driver == NULL ){ 
        std::cout << "dataset: " << m_dataset << ", driver: " << m_driver << std::endl;
        return false; 
    }

    // set the image to zero
    img = 0;

    std::cout << "input : " << GetOpenCVTypeName(m_type) << std::endl;
    std::cout << "output: " << GetOpenCVTypeName(img.type()) << std::endl;

    // iterate over each raster band
    // note that OpenCV does bgr rather than rgb
    int nChannels = m_dataset->GetRasterCount();
    const GDALDataType gdalType = m_dataset->GetRasterBand(1)->GetRasterDataType();
    int nRows, nCols;

    if( nChannels > img.channels() ){ 
        nChannels = img.channels(); 
    }

    for( int c = 0; c<nChannels; c++ ){

        // get the GDAL Band
        GDALRasterBand* band = m_dataset->GetRasterBand(c+1);

        // make sure the image band has the same dimensions as the image
        if( band->GetXSize() != m_width || band->GetYSize() != m_height ){ return false; }
        
        // grab the raster size
        nRows = band->GetYSize();
        nCols = band->GetXSize();
        
        // create a temporary scanline pointer to store data
        double* scanline = new double[nCols]; 

        // iterate over each row and column
        for( int y=0; y<nRows; y++ ){

            // get the entire row
            band->RasterIO( GF_Read, 0, y, nCols, 1, scanline, nCols, 1, GDT_Float64, 0, 0);

            // set inside the image
            for( int x=0; x<nCols; x++ ){
            
                // set depending on image types
                //   given boost, I would use enable_if to speed up.  Avoid for now.
                write_pixel( scanline[x], gdalType, nChannels, img, y, x, c );

            }
        }

        // delete our temp pointer
        delete [] scanline;


    }

    return true;
}


/**
 * Read image header
*/
bool GdalDecoder::readHeader(){

    std::cout << "gdal decoder readHeader" << std::endl;

	// load the dataset 
    m_dataset = (GDALDataset*) GDALOpen( m_filename.c_str(), GA_ReadOnly);
	
    // if dataset is null, then there was a problem
	if( m_dataset == NULL ){
        std::cout << "GDALOpen returned a null object." << std::endl;
		return false;
	}
	
	// make sure we have pixel data inside the raster
	if( m_dataset->GetRasterCount() <= 0 ){
        std::cout << "File has no pixel data." << std::endl;
		return false;
	}
	
	//extract the driver infomation
    m_driver = m_dataset->GetDriver();
    
    // if the driver failed, then exit
    if( m_driver == NULL ){ 
        std::cout << "Driver failed to load" << std::endl;
        return false; 
    }
    

    // get the image dimensions
    m_width = m_dataset->GetRasterXSize();
    m_height= m_dataset->GetRasterYSize();
    
    // make sure we have at least one band/channel
    if( m_dataset->GetRasterCount() <= 0 ){
        std::cout << "No raster band found." << std::endl;
        return false;
    }

    // convert the datatype to opencv
    int tempType = gdal2opencv( m_dataset->GetRasterBand(1)->GetRasterDataType(), m_dataset->GetRasterCount());
    if( tempType == -1 ){
        std::cout << "type not recognized. Load failed." << std::endl;
        return false;
    }
    m_type = tempType;
    
    return true;
}

/**
 * Close the module
*/
void GdalDecoder::close(){
    
    std::cout << "gdal decoder close" << std::endl;

    GDALClose((GDALDatasetH)m_dataset);
    m_dataset = NULL;
    m_driver = NULL;
}

/**
 * Create a new decoder
*/
ImageDecoder GdalDecoder::newDecoder()const{
    return makePtr<GdalDecoder>();
}

/**
 * Test the file signature
*/
bool GdalDecoder::checkSignature( const String& signature )const{

    std::cout << "gdal decoder check signature" << std::endl;
    std::cout << "sig: " << signature.c_str() << std::endl;
    
    // look for NITF
    std::string str = signature.c_str();
    if( str.substr(0,4).find("NITF") != std::string::npos ){
        return true;
    }
    
    return false;
}


} /// End of cv Namespace
