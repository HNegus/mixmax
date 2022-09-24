cmake_minimum_required(VERSION 3.0.0)
project(MAX VERSION 0.1.0)

# include(CTest)
# enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB
    SOURCES
    src/*.cpp
    )

# set(OpenCV_ROOT "./src/opencv")
# set(OpenCV_INCLUDE_DIRS "${OpenCV_ROOT}/include")
# set(OpenCV_LIBRARIES "${OpenCV_ROOT}/lib")

set(OPENCV_LIBS
    # opencv_calib3d
    opencv_core
    # opencv_dnn
    # opencv_features2d
    # opencv_flann
    opencv_highgui
    # opencv_imgcodecs
    opencv_imgproc
    # opencv_ml
    # opencv_objdetect
    # opencv_photo
    # opencv_shape
    # opencv_stitching
    # opencv_superres
    # opencv_video
    opencv_videoio
    # opencv_videostab
    )

# include_directories(opencv/include)
include_directories(src/opencv/modules/core/include
                    src/opencv/build/include
                    src/opencv/build
                    src/opencv/include
                    src/opencv/modules/calib3d/include
                    src/opencv/modules/core/include
                    src/opencv/modules/dnn/include
                    src/opencv/modules/features2d/include
                    src/opencv/modules/flann/include
                    src/opencv/modules/highgui/include
                    src/opencv/modules/imgcodecs/include
                    src/opencv/modules/imgproc/include
                    src/opencv/modules/ml/include
                    src/opencv/modules/objdetect/include
                    src/opencv/modules/photo/include
                    src/opencv/modules/stitching/include
                    src/opencv/modules/video/include
                    src/opencv/modules/videoio/include
                    )
# include_directories(${OpenCV_INCLUDE_DIRS})


# find_library(opencv ${OpenCV_LIBS}src/opencv/build/lib)

link_directories(src/opencv/build/lib)
                #  src/opencv/build/3rdparty/ippicv/ippicv_lnx/icv/lib/intel64/)
add_executable(MAX ${SOURCES})


# find_package(OpenCV REQUIRED)

target_link_libraries(MAX 
    PUBLIC ${OPENCV_LIBS}
    # PRIVATE opencv_core
    )

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
