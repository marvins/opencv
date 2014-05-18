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
#include <string>

namespace cv{


/**
 * GDAL Decoder Constructor
*/
GdalDecoder::GdalDecoder(){

    std::cout << "gdal decoder constructor" << std::endl;
    
    // set a dummy signature
    m_signature="000000000000";
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
 * read data
*/
bool GdalDecoder::readData( Mat& img ){

    std::cout << "gdal decoder readData" << std::endl;

    return true;
}


/**
 * Read image header
*/
bool GdalDecoder::readHeader(){

    std::cout << "gdal decoder readHeader" << std::endl;
   
    /// Register the driver
    GDALAllRegister();

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

    // get the image dimensions
    m_width = m_dataset->GetRasterXSize();
    m_height= m_dataset->GetRasterYSize();
    
    // make sure we have at least one band/channel
    if( m_dataset->GetRasterCount() <= 0 ){
        std::cout << "No raster band found." << std::endl;
        return false;
    }

    // convert the datatype to opencv
    int tempType = CV_8U;
    switch( m_dataset->GetRasterBand(1)->GetRasterDataType()){

        /// UInt8
        case GDT_Byte:
            tempType = CV_8U;
            break;
        
        /// UInt16
        case GDT_UInt16:
            tempType = CV_16U;
            break;

        /// Int16
        case GDT_Int16:
            tempType = CV_16S;
            break;

        /// UInt32
        case GDT_UInt32:
        case GDT_Int32:
            tempType = CV_32S;
            break;

        default:
            std::cout << "Unknown GDAL Data Type" << std::endl;
            std::cout << "Type: " << GDALGetDataTypeName( m_dataset->GetRasterBand(1)->GetRasterDataType()) << std::endl;
            return false;
    }

    std::cout << "Type: " << GDALGetDataTypeName( m_dataset->GetRasterBand(1)->GetRasterDataType()) << std::endl;
    // grayscale
    if( m_dataset->GetRasterCount() == 1 ){
        std::cout << "Single" << std::endl;
        return false;
    }

    /// rgb / 3-channel
    else if( m_dataset->GetRasterCount() == 3 ){

        std::cout << "Three" << std::endl;
        return false;
    }
    else {
        return false;
    }

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
