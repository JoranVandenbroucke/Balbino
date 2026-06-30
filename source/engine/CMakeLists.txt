set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(LIBRARY_NAME BalbinoEngine)
add_library(${LIBRARY_NAME})
target_sources(${LIBRARY_NAME} PRIVATE
        application.cpp

        PRIVATE
        FILE_SET application_headers TYPE HEADERS
        BASE_DIRS
        ${CMAKE_SOURCE_DIR}
        FILES
        application.hpp
        balbino.hpp
        balbino_main.hpp
        entry_point.hpp
)

target_include_directories(${LIBRARY_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/embedded
)

target_compile_definitions(${LIBRARY_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_23)
set_target_properties(${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    CXX_SCAN_FOR_MODULES  ON
)


include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set_compile_flags(${LIBRARY_NAME})
select_best_stl(${LIBRARY_NAME})
select_best_linker(${LIBRARY_NAME})

add_subdirectory(math)
add_subdirectory(renderer)

add_dependencies(${LIBRARY_NAME}
        FawnVision::FawnVision
)

target_link_libraries(${LIBRARY_NAME} PRIVATE
        FawnVision::FawnVision
)
