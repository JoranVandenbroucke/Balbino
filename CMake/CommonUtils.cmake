function(set_compile_options target type)
    target_compile_options(${target} ${type}
            $<$<CXX_COMPILER_ID:MSVC>:
            /Zi /W4 /WX /MP $<$<CONFIG:Debug,DebugEditor>:${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING}>
            >
            $<$<CXX_COMPILER_ID:GNU>:
            -Wall -Wextra -Werror
            >
            $<$<CXX_COMPILER_ID:Clang>:
            -Wall -Wextra -Werror
            >
    )
    target_link_options(${target} ${type}
            $<$<CXX_COMPILER_ID:MSVC>:
            /INCREMENTAL:NO /DEBUG:FULL /OPT:ICF
            $<$<CONFIG:Debug,DebugEditor>:/SUBSYSTEM:CONSOLE>
            $<$<CONFIG:Release,ReleaseEditor>:/SUBSYSTEM:WINDOWS >
            >
            $<$<CXX_COMPILER_ID:GNU>:
            -fms-extension
            $<$<CONFIG:Debug,DebugEditor>:-Wl,--incremental -g -Wl,--subsystem,console>
            $<$<CONFIG:Release,ReleaseEditor>:-Wl,--no-incremental -g -Wl,--subsystem,windows -Wl,--gc-sections -Wl,--icf=all>
            >
            $<$<CXX_COMPILER_ID:Clang>:
            -fms-extension
            $<$<CONFIG:Debug,DebugEditor>:-Wl,--incremental -g -Wl,--subsystem,console>
            $<$<CONFIG:Release,ReleaseEditor>:-Wl,--no-incremental -g -Wl,--subsystem,windows -Wl,--gc-sections -Wl,--icf=all>
            >
    )

    target_compile_features(${target} ${type}
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
endfunction()

function(set_definitions target type)
    target_compile_definitions(${target} ${type}
            $<$<BOOL:${BALBINO_EDITOR}>:BALBINO_EDITOR>

            BL_BUILD_LIB
            _CONSOLE
            UNICODE
            _UNICODE
    )
endfunction()

function(set_precompiled_header target)
    #    target_precompile_headers(${target} ${type}
    #            <algorithm>
    #            <array>
    #            <cmath>
    #            <cstdint>
    #            <format>
    #            <iostream>
    #            <unordered_map>
    #            <vector>
    #    )
endfunction()