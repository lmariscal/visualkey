cmake_minimum_required(VERSION 3.2)

project(visualkey CXX)

option(MONO_ROOT "Installed Mono path" "")
option(MONO_INCLUDE "Installed Mono include dir path" "")
option(MONO_LINK "Where to locate Mono libs" "")
option(MONO_LIBS "Libraries to link against" "")
option(MONO_LIB "Installed Mono lib dir path" "")
option(MONO_ETC "Installed Mono etc dir path" "")

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
  set(CMAKE_C_FLAGS "-fuse-ld=lld -Wno-unused-command-line-argument")
  set(CMAKE_CXX_FLAGS "-fuse-ld=lld -Wno-unused-command-line-argument")
endif()

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

if(NOT CMAKE_C_COMPILE_OBJECT)
  if (WIN32)
    set(CMAKE_C_COMPILE_OBJECT ${CMAKE_CXX_COMPILE_OBJECT})
  elseif (APPLE)
    set(CMAKE_C_COMPILE_OBJECT "clang <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
  else()
    set(CMAKE_C_COMPILE_OBJECT "gcc <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
  endif()
endif()

add_subdirectory(third)
add_subdirectory(src)
