cmake_minimum_required(VERSION 3.27)
set(PROJECT_NAME Deer)
set(ROOT_NAMESPACE ${PROJECT_NAME})
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Engine Engine)
add_dependencies(${PROJECT_NAME}
        BalbinoEngine
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        [["Balbino.h"]]
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/Source/Engine
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        BalbinoEngine
)

target_compile_features(${PROJECT_NAME} PRIVATE
        c_static_assert
        c_std_17
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

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Installs the executable in the binary directory
)