cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(diffmap C)

if(NOT CMAKE_BUILD_TYPE)
	message("Warning: CMAKE_BUILD_TYPE is not set.\nYou are advised to set it to one of Debug, Release, RelWithDebInfo or MinSizeRel.\nSee https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Werror")

find_path(LIBEXIF_INCLUDE_DIR DIR libexif/exif-data.h HINTS /usr/local/include)
include_directories(${LIBEXIF_INCLUDE_DIR})

find_path(LIBEXIF_LIB_DIR DIR libexif.so HINTS /usr/local/lib)
link_directories(${LIBEXIF_LIB_DIR})

add_executable(exifsort exifsort.c)
target_link_libraries(exifsort exif)
