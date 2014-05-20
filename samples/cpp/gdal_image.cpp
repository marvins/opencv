/**
 * gdal_image.cpp -- Load GIS data into OpenCV Containers using the Geospatial Data Abstraction Library
*/

/// OpenCV Headers
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/// IO stream
#include <iostream>

using namespace std;

/**
 * Main Function
*/
int main( int argc, char* argv[] ){

    /**
     * Check input arguments
    */
    if( argc < 3 ){
        cout << "usage: " << argv[0] << " <image> <dem>" << endl;
    }

    /// load the image (note that we don't have the projection information.  You will
    /// need to load that yourself or use the full GDAL driver.
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_LOAD_GDAL | cv::IMREAD_COLOR );

    /// load the dem model
    cv::Mat dem = cv::imread(argv[2], cv::IMREAD_LOAD_GDAL | cv::IMREAD_GRAYSCALE );
    cout << "DEM loaded" << endl;

    cout << "Image: " << image.rows << " x " << image.cols << " with " << image.channels() << endl;
    cout << "Dem  : " << dem.rows << " x " << dem.cols << " with " << dem.channels() << endl;

    if( image.rows <= 0 || image.cols <= 0 ){ return 1; }
    if( dem.rows <= 0 || dem.cols <= 0 ){ return 1; }

    cv::imshow("image",image);
    cv::waitKey(5000);

    // ideas
    // 1.  load normal image
    // 2.  load elevation data and create heat map
    // 3.  overlay route onto map using geographic corners

    return 0;
}
