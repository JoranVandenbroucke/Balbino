cmake_minimum_required(VERSION 4.0.1)
project(BalbinoEngine VERSION 0.3.3 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_SCAN_FOR_MODULES ON)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
set(CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_COMPILER_IMPORT_STD ON)

add_library(${PROJECT_NAME}
)
target_source(${PROJECT_NAME} PRIVATE
        application.cpp

        PRIVATE
        FILE_SET application_headers TYPE HEADERS
        BASE_DIRS
        ${CMAKE_SOURCE_DIR}
        FILES
        application.hpp
        balbino.hpp
        balbino_main.hpp
        entryPoint.hpp

        PRIVATE
        FILE_SET embedded_headers TYPE HEADERS
        BASE_DIRS
        ${CMAKE_SOURCE_DIR}/embedded
        FILES
        ${CMAKE_SOURCE_DIR}/embedded/images.hpp
        ${CMAKE_SOURCE_DIR}/embedded/shaders.hpp
)

add_subdirectory(math)
add_subdirectory(renderer)
add_dependencies(${PROJECT_NAME}
        FawnVision::FawnVision
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        FawnVision::FawnVision
)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/embedded")

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

include(compiler_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set(COMPILE_FUNCTION_NAME "set_${CMAKE_CXX_COMPILER_ID}_compile_flags")
if(EXISTS ${COMPILE_FUNCTION_NAME})
    execute_process(COMMAND ${COMPILE_FUNCTION_NAME} ${PROJECT_NAME})
endif()
select_best_stl(${PROJECT_NAME})
select_best_linker(${PROJECT_NAME})
