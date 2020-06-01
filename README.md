
# Simple tracker and visualizer
## Written in C++, using Qt and OpenCv
##### Requirements: Cmake 3.17.2 or higher Qt 5.14 or higher, Opencv 4.3.0 or higher, OpenMp
######  if openmp not found: https://stackoverflow.com/questions/48825416/missing-openmp-c-flags-openmp-c-lib-names

**Build with cmake:**   
cmake -D CMAKE_BUILD_TYPE="DEBUG"   
-D CMAKE_PREFIX_PATH="*qt path to compiler mscv or clang*"  
-D OpenCV_DIR="*open cv config cmake path*"  
-D Python3_ROOT_DIR="*path to python3*"
-D ARCHITECTURE=x64  
-G "*Cmake generator like Visual Studio 15 2017 or Xcode*"  
-D CMAKE_INSTALL_PREFIX="*install folder path*" .. 

## ToDo:
####   general
create testing framework  
logging  
install for windows/mac/linux  
#### 2d tracking
integrate a python tracker  
tracking fish from videos  
live tracking (with camera feed)  
output centroid coordinates, with ids, timestamp  
#### 3d tracking 
camera calibration  
stitching two videos(3d)  
#### visualizer
network for real time transmitting  
visualizing movement of tracked centroids (2d)  
modelling fish (gpu?) (3d)  
