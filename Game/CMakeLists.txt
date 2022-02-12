cmake_minimum_required(VERSION 3.21.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Set Project Name
set(PROJECT_NAME Game)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# sundirectories
add_subdirectory(../Balbino ../Balbino)

# project setting
add_executable(${PROJECT_NAME} scr/main.cpp)

target_precompile_headers(${PROJECT_NAME} PUBLIC scr/pch.h)
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

# libaries
target_link_libraries(${PROJECT_NAME} PUBLIC Balbino)