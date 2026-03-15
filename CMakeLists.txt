cmake_minimum_required(VERSION 4.01)
project(Balbino VERSION 0.0.1 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_subdirectory(source/game)
