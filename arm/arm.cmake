INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME arm-linux)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-linux-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-linux-g++ GNU)
SET(CMAKE_C_LINK_EXECUTABLE "arm-linux-ld")
