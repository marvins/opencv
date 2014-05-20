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

    ///
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_LOAD_GDAL );

    cout << image.rows << " x " << image.cols << endl;
    if( image.rows <= 0 || image.cols <= 0 ){ return 1; }
    
    cv::imshow("image",image);
    cv::waitKey(5000);

    // ideas
    // 1.  load normal image
    // 2.  load elevation data and create heat map
    // 3.  overlay route onto map using geographic corners

    return 0;
}
