cmake_minimum_required(VERSION 3.0)
project(syrup-term)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(SYRUP_BUILD_TEST "build tests" off)

find_package(syrup-buffer REQUIRED)


#include_directories(include vendor/syrup-buffer/include)


#add_subdirectory(vendor/syrup-buffer)
add_subdirectory(src)

add_subdirectory(example)