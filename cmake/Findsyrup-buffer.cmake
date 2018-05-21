# - Try to find syrup-buffer
# Once done this will define
#  SYRUP_BUFFER_FOUND - System has syrup-buffer
#  SYRUP_BUFFER_INCLUDE_DIRS - The syrup-buffer include directories
#  SYRUP_BUFFER_LIBRARIES - The libraries needed to use syrup-buffer
#  SYRUP_BUFFER_DEFINITIONS - Compiler switches required for using syrup-buffer

find_package(PkgConfig)
pkg_check_modules(PC_SYRUP_BUFFER QUIET csystem)
set(MBEDTLS_DEFINITIONS ${PC_SYRUP_BUFFER_CFLAGS_OTHER})

find_path(SYRUP_BUFFER_INCLUDE_DIR csystem.h
          HINTS ${PC_SYRUP_BUFFER_INCLUDEDIR} ${PC_SYRUP_BUFFER_INCLUDE_DIRS}
          PATH_SUFFIXES csystem)

find_library(SYRUP_BUFFER_LIBRARY NAMES csystem csystem_crypto
             HINTS ${PC_SYRUP_BUFFER_LIBDIR} ${PC_SYRUP_BUFFER_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
##find_package_handle_standard_args(CSYSTEM ""
  ##                                SYRUP_BUFFER_LIBRARY SYRUP_BUFFER_INCLUDE_DIR)

if(NOT SYRUP_BUFFER_INCLUDE_DIR)
    add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/syrup-buffer)
    include_directories(${PROJECT_SOURCE_DIR}/vendor/syrup-buffer/include)
    set(SYRUP_BUFFER_LIBRARY syrup-buffer)
    set(SYRUP_BUFFER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/vendor/syrup-buffer/include)
    set(SYRUP_BUFFER_FOUND on)
    message(STATUS "Using local syrup-buffer")
else()
    message(STATUS "Found syrup-buffer")
    set(SYRUP_BUFFER_FOUND on)
endif(NOT SYRUP_BUFFER_INCLUDE_DIR)

mark_as_advanced(SYRUP_BUFFER_INCLUDE_DIR SYRUP_BUFFER_LIBRARY)

set(SYRUP_BUFFER_LIBRARIES ${SYRUP_BUFFER_LIBRARY} )
set(SYRUP_BUFFER_INCLUDE_DIRS ${SYRUP_BUFFER_INCLUDE_DIR} )