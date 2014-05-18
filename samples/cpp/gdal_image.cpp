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
    cv::Mat image = cv::imread(argv[1]);

    cout << image.rows << " x " << image.cols << endl;
    cv::imshow("image",image);
    cv::waitKey(10);

    return 0;
}
