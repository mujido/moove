project(moove_top)
cmake_minimum_required(VERSION 3.10.0)


if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
else()
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
endif()
conan_basic_setup(TARGETS)

find_package(BISON REQUIRED)

add_subdirectory(src)
add_subdirectory(moosh)
