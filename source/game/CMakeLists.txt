cmake_minimum_required(VERSION 4.3.0)
set(CMAKE_CXX_COMPILER_IMPORT_STD ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
project(Deer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_MODULE_STD 1)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/source)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    CXX_SCAN_FOR_MODULES  ON
)

include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set_compile_flags(${PROJECT_NAME})
select_best_stl(${PROJECT_NAME})
select_best_linker(${PROJECT_NAME})

add_subdirectory(${CMAKE_SOURCE_DIR}/source/engine engine)

add_dependencies(${PROJECT_NAME}
        FawnVision::FawnVision
        BalbinoEngine
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/embedded
        ${CMAKE_SOURCE_DIR}/source/engine
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        FawnVision::FawnVision
        BalbinoEngine
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Installs the executable in the binary directory
)
