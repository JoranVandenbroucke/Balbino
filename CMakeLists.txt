cmake_minimum_required(VERSION 4.01)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
set(CMAKE_CXX_MODULE_STD 1)
project(Balbino VERSION 0.1.1 LANGUAGES C CXX)

option(BALBINO_USE_SANITIZERS "Use Sanitizer" OFF)
option(BALBINO_EDITOR "Use Sanitizer" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_subdirectory(source/game)