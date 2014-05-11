/**
 * gdal_image.cpp -- Load GIS data into OpenCV Containers using the Geospatial Data Abstraction Library
*/

/// OpenCV Headers
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


/**
 * Main Function
*/
int main( int argc, char* argv[] ){

    ///
    cv::Mat image = cv::imread(argv[1]);

    cv::imshow("image",image);
    cv::waitKey(0);

    return 0;
}
