project(moove_top)
cmake_minimum_required(VERSION 3.12.0)


if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
else()
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
endif()
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/Zc:__cplusplus /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAS} -Wall")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_subdirectory(src)
add_subdirectory(moosh)
