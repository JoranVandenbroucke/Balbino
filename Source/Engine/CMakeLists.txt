cmake_minimum_required(VERSION 3.28)
project(BalbinoEngine VERSION 0.3.2 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(${PROJECT_NAME}
        Embedded/Images.hpp
        Embedded/Shaders.hpp
        Application.cpp
        Application.h
        Balbino.h
        Balbino_main.h
        EntryPoint.h
        Versions.hpp
)

include(${CMAKE_SOURCE_DIR}/CMake/SetupSDL3.cmake)
setup_sdl3(${PROJECT_NAME})

add_subdirectory(Math)
add_subdirectory(Renderer)
add_dependencies(${PROJECT_NAME}
        FawnVision::FawnVision
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        FawnVision::FawnVision
)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Embedded")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_options(${PROJECT_NAME}
        PRIVATE
        $<$<CONFIG:Debug>:${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING}>
        $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX /MP $<$<CONFIG:Debug>:${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING}>>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
        PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
        #todo -embed-dir="${CMAKE_SOURCE_DIR}/Embedded"
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)

target_compile_features(${PROJECT_NAME} PRIVATE
        c_function_prototypes
        c_restrict
        c_static_assert
#        c_std_23
        c_variadic_macros
        cxx_aggregate_default_initializers
        cxx_alignof
        cxx_attributes
        cxx_auto_type
        cxx_binary_literals
        cxx_constexpr
        cxx_contextual_conversions
        cxx_defaulted_functions
        cxx_defaulted_move_initializers
        cxx_delegating_constructors
        cxx_deleted_functions
        cxx_enum_forward_declarations
        cxx_explicit_conversions
        cxx_final
        cxx_generalized_initializers
        cxx_generic_lambdas
        cxx_inheriting_constructors
        cxx_inline_namespaces
        cxx_lambda_init_captures
        cxx_lambdas
        cxx_long_long_type
        cxx_noexcept
        cxx_nonstatic_member_init
        cxx_nullptr
        cxx_override
        cxx_range_for
        cxx_raw_string_literals
        cxx_right_angle_brackets
        cxx_rvalue_references
        cxx_sizeof_member
        cxx_static_assert
        cxx_std_23
        cxx_strong_enums
        cxx_thread_local
        cxx_uniform_initialization
        cxx_unrestricted_unions
        cxx_user_literals
)
