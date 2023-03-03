cmake_minimum_required(VERSION 3.24)
project(MLXApi LANGUAGES C CXX)
add_library(MLXApi STATIC
headers/MLX90640_API.h
headers/MLX90640_BLEProxy.h
headers/MLX90640_Registers.h
headers/MLX90640_I2C_Driver.h

functions/MLX90640_API.c
functions/MLX90640_BLEProxy.cpp
functions/MLX90640_Test.c
)


set_target_properties(MLXApi PROPERTIES
FRAMEWORK TRUE
FRAMEWORK_VERSION CXX
PUBLIC_HEADER "headers/MLX90640_API.h;headers/MLX90640_BLEProxy.h;")

target_include_directories(MLXApi PUBLIC headers)
target_compile_features(MLXApi PUBLIC cxx_std_17)
set(DCMAKE_TOOLCHAIN_FILE ../ios.toolchain.cmake)
set(DPLATFORM OS64)
