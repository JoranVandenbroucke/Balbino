cmake_minimum_required(VERSION 4.0.1)
project(Deer VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_SCAN_FOR_MODULES ON)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
set(CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_COMPILER_IMPORT_STD ON)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/source)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set(COMPILE_FUNCTION_NAME "set_${CMAKE_CXX_COMPILER_ID}_compile_flags")
if(EXISTS ${COMPILE_FUNCTION_NAME})
    execute_process(COMMAND ${COMPILE_FUNCTION_NAME} ${PROJECT_NAME})
endif()
select_best_stl(${PROJECT_NAME})
select_best_linker(${PROJECT_NAME})

add_subdirectory(${CMAKE_SOURCE_DIR}/source/engine Engine)

add_dependencies(${PROJECT_NAME}
        FawnVision::FawnVision
        BalbinoEngine
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/embedded
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        FawnVision::FawnVision
        BalbinoEngine
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Installs the executable in the binary directory
)
