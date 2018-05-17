cmake_minimum_required(VERSION 3.0)
project(syrup-term)

option(SYRUP_BUILD_TEST "build tests" off)

include_directories(include vendor/syrup-buffer/include)


add_subdirectory(vendor/syrup-buffer)
add_subdirectory(src)

add_subdirectory(example)