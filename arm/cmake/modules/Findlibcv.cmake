MESSAGE(STATUS "Using Findlibcv.cmake...")
FIND_PATH(
	LIBCV_CXX_INCLUDE_DIR
	opencv2/opencv.hpp
	/opt/sysroot/usr/local/include/
	)

FIND_LIBRARY(
	LIBCV_CXX_LIBRARIES NAMES opencv_core opencv_highgui opencv_imgproc
	PATHS /opt/sysroot/usr/local/lib
	)
