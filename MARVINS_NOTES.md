
Objective:
==========

* Add GDAL Support to OpenCV to allow reading and writing of image raster data.
* Add comments to driver section to make adding more file types easier.

Update repo to OpenCV Master on Itseez
======================================

1.  Add the repo to the remote list
    git remote add upstream https://github.com/Itseez/opencv.git

2.  Fetch the branches
    git fetch

3.  Sync with master
    git merge upstream/master


Status
======

Status 5/10/2014
#.  Started adding comments to the driver sections
    - imread is located in modules/highgui/src/loadsave.cpp
    - gdal driver will be located in modules/highgui/src/grfmt_gdal.hpp/.cpp
