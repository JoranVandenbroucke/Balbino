cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)

set(PROJECT_NAME hdrloader)

project(${PROJECT_NAME} LANGUAGES C CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
            "$<$<CONFIG:>:/MD>" # use MTd for Debug
            "$<$<CONFIG:DEBUG>:/MDd>" # use MTd for Debug
            "$<$<CONFIG:RELEASE>:/MD>"  # use MT for Release
    )
endif()

add_library(${PROJECT_NAME} STATIC hdrloader.cpp hdrloader.h)