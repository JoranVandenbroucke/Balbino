cmake_minimum_required(VERSION 3.21.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Balbino VERSION 1.0.0 LANGUAGES CXX)



file(GLOB source
        scr/*.cpp scr/*.h
        scr/Input/*.cpp scr/Input/*.h
        scr/Managers/*.cpp scr/Managers/*.h
        scr/Scene/*.cpp scr/Scene/*.h
        scr/Renderer/*.cpp scr/Renderer/*.h)

add_library(Balbino ${source})

target_precompile_headers(Balbino PUBLIC scr/pch.h)
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

install(
        TARGETS Balbino
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)