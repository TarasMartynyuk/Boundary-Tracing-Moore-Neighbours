cmake_minimum_required(VERSION 3.8)
project(BoundaryTracing)

set(CMAKE_CXX_STANDARD 11)

# Where to find CMake modules and OpenCV
set(OpenCV_DIR "C:\\CodeProjects\\libs\\opencv-3.4.0\\mingw-build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(BoundaryTracing main.cpp mat_from_file.h mat_from_file.cpp
        boundary_tracing.cpp boundary_tracing.h PixelCoords2D.h PixelCoords2D.cpp finding_non_zero_pixels.cpp finding_non_zero_pixels.h)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(BoundaryTracing ${OpenCV_LIBS})