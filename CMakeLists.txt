cmake_minimum_required(VERSION 3.14.0)

set(PROJ_NAME "FA3dConverter")
project(${PROJ_NAME} VERSION 1.0.0 LANGUAGES CXX)

add_library(FA3dConverter INTERFACE)

target_include_directories(FA3dConverter INTERFACE include)

target_compile_features(FA3dConverter INTERFACE cxx_std_20)

set(CMAKE_CXX_STANDARD 20)

if(CONV_COMPILE_TESTS)
    add_subdirectory(tests)
endif()
