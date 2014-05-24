.. _Raster_IO_GDAL:
    

Reading Geospatial Raster files with GDAL
*****************************************

Geospatial raster data is a heavily used product in Geographic Information
Systems and Photogrammetry.  Raster data typically can represent imagery 
and Digital Elevation Models (DEM).  The standard library for loading
GIS imagery is the Geographic Data Abstraction Library (GDAL).  In this example, we
will show techniques for loading GIS raster formats using native OpenCV functions.
In addition, we will show some an example of how OpenCV can use this data for 
novel and interesting purposes.

Goals
=====

The primary objectives for this tutorial:

.. container:: enumeratevisibleitemswithsquare
    
    + How to use OpenCV imread to load satellite imagery.
    + How to use OpenCV imread to load SRTM Digital Elevation Models
    + Given the corner coordinates of both the image and DEM, correllate the elevation data to the image to find elevations for each pixel.
    + Show a basic, easy-to-implement example of a terrain heat map.
    + Show a basic use of DEM data coupled with ortho-rectified imagery.


Code
====

.. literalinclude:: ../../../../samples/cpp/tutorial_code/HighGUI/GDAL_IO/gdal-image.cpp
   :language: cpp
   :linenos:
   :tab-width: 4


Explanation
============

This demonstration uses the default OpenCV :ocv:func:`imread` function.  The primary difference is that in order to force GDAL to load the 
image, you must use the appropriate flag.

.. code-block:: cpp

    cv::Mat image = cv::imread( argv[1], cv::IMREAD_LOAD_GDAL );

When loading digital elevation models, the actual numeric value of each pixel is essential
and cannot be scaled.  With image data, pixels represented as doubles with a value of 1 is equal to an unsigned character image
with a value of 255.  With terrain data, the pixel value represents the elevation in meters.  In order to ensure that OpenCV preserves this data,
use the GDAL flag in imread with the ANYDEPTH flag.

.. code-block:: cpp

    cv::Mat dem = cv::imread( argv[2], cv::IMREAD_LOAD_GDAL | cv::IMREAD_ANYDEPTH );


If you know the DEM model file before loading, you should verify the image is single channel with the appropriate pixel depth.  NASA or the DOD can provide
the input types for various elevation model types.  SRTM and DTED both are signed shorts. 


Result
======


