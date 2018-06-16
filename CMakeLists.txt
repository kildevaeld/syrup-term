cmake_minimum_required(VERSION 3.0)
project(syrup-term)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(SYRUP_BUILD_EXAMPLE "build example" off)

find_package(syrup-buffer REQUIRED)

include_directories(include ${SYRUP_BUFFER_INCLUDE_DIRS})

add_subdirectory(src)


if(SYRUP_BUILD_EXAMPLE)
    add_subdirectory(example)
endif(SYRUP_BUILD_EXAMPLE)