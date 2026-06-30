set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(EXE_NAME Deer)
add_executable(${EXE_NAME})
target_sources(${EXE_NAME} PRIVATE main.cpp)

target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_compile_definitions(${EXE_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${EXE_NAME} PRIVATE cxx_std_23)
set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    CXX_SCAN_FOR_MODULES  ON
)

include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set_compile_flags(${EXE_NAME})
select_best_stl(${EXE_NAME})
select_best_linker(${EXE_NAME})

add_subdirectory(${CMAKE_SOURCE_DIR}/source/engine engine)

add_dependencies(${EXE_NAME}
        FawnVision::FawnVision
        BalbinoEngine
)

target_include_directories(${EXE_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/embedded
        ${CMAKE_SOURCE_DIR}/source/engine
)

target_link_libraries(${EXE_NAME} PRIVATE
        FawnVision::FawnVision
        BalbinoEngine
)

install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Installs the executable in the binary directory
)

if (WIN32)
    install(FILES $<TARGET_FILE:SDL3::SDL3>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if (MSVC)
        get_filename_component(MSVC_TOOLS_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
        find_file(ASAN_DLL
            NAMES clang_rt.asan_dynamic-x86_64.dll
            HINTS "${MSVC_TOOLS_DIR}"
            NO_DEFAULT_PATH
        )
        if (ASAN_DLL)
            install(FILES "${ASAN_DLL}"
                DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
        endif()
    endif()
endif()