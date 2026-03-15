function(set_MSVC_compile_flags target)
    # Common flags reused nyacross configurations
    set(common_compile_opts /W4 /WX /MP /permissive-)
    set(common_defs _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS _ENABLE_EXTENDED_ALIGNED_STORAGE=1)

    target_compile_options(${target} PRIVATE
            # --- Debug ---
            $<$<CONFIG:Debug>:
            ${common_compile_opts}
            /Od /Zi /DEBUG:FULL /RTC1 /fsanitize=address /permissive-
            >

            # --- Release ---
            $<$<CONFIG:Release>:
            ${common_compile_opts}
            /Oitx /GL /LTCG /Gy /Gw /Qpar /fp:fast /GS-
            /favor:blend /arch:SSE2 /Zc:inline /Zc:__cplusplus
            >

            # --- RelWithDebInfo ---
            $<$<CONFIG:RelWithDebInfo>:
            ${common_compile_opts}
            /O2 /Oi /fp:fast /Gy /arch:SSE2 /Zi /DEBUG:FULL
            /EHsc /GS /Zf /Zc:inline
            >

            # --- MinSizeRel ---
            $<$<CONFIG:MinSizeRel>:
            /O1 /Os /Oy /Gy /Gw /GL /LTCG /GS- /GR- /EHs-c-
            /fp:precise /arch:SSE2 /Oi- /favor:blend /W3 /MP
            /Zc:inline /Zc:threadSafeInit- /nologo /diagnostics:column
            >
    )

    target_link_options(${target} PRIVATE
            # --- Debug ---
            $<$<CONFIG:Debug>:
            /DEBUG:FULL
            /INCREMENTAL
            /OPT:NOREF
            /OPT:NOICF
            /PDBALTPATH:%_PDB%
            >

            # --- Release + MinSizeRel ---
            $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:
            /LTCG
            /INCREMENTAL:NO
            /OPT:REF
            /OPT:ICF
            /ZI
            /NODEFAULTLIB:msvcrtd
            >

            # --- RelWithDebInfo ---
            $<$<CONFIG:RelWithDebInfo>:
            /DEBUG:FULL /INCREMENTAL /OPT:NOREF /OPT:NOICF /fsanitize=address
            >

            # --- MinSizeRel specific additions ---
            $<$<CONFIG:MinSizeRel>:
            /MERGE:.rdata=.text /MERGE:.data=.text /OPT:NOWIN98
            /SUBSYSTEM:CONSOLE /RELEASE
            >
    )

    target_compile_definitions(${target} PRIVATE
            # --- Debug ---
            $<$<CONFIG:Debug>:
            _ITERATOR_DEBUG_LEVEL=2
            _CRTDBG_MAP_ALLOC
            ${common_defs}
            >

            # --- Release ---
            $<$<CONFIG:Release>:
            NDEBUG
            _SECURE_SCL=0
            _HAS_ITERATOR_DEBUGGING=0
            _ITERATOR_DEBUG_LEVEL=0
            _HAS_EXCEPTIONS=0
            _HAS_RTTI=0
            ${common_defs}
            >

            # --- RelWithDebInfo ---
            $<$<CONFIG:RelWithDebInfo>:
            _ITERATOR_DEBUG_LEVEL=1
            _SECURE_SCL=1
            _HAS_ITERATOR_DEBUGGING=1
            ${common_defs}
            >

            # --- MinSizeRel ---
            $<$<CONFIG:MinSizeRel>:
            NDEBUG
            _SECURE_SCL=0
            _HAS_ITERATOR_DEBUGGING=0
            _ITERATOR_DEBUG_LEVEL=0
            _HAS_EXCEPTIONS=0
            _HAS_RTTI=0
            _STATIC_CPPLIB
            _NO_CRT_STDIO_INLINE
            ${common_defs}
            >
    )
endfunction()

# Clang Debug
function(set_CLANG_compile_flags target)
    # --- Common sets ---
    set(common_compile_opts
            -Wall -Wextra -Wpedantic -Wconversion -Werror
            -fcolor-diagnostics -fansi-escape-codes
            -fno-omit-frame-pointer
    )

    set(common_defs
            _FORTIFY_SOURCE=2
            _FILE_OFFSET_BITS=64
    )

    # ------------------ Compile Options ------------------
    target_compile_options(${target} PRIVATE
            # Debug
            $<$<CONFIG:Debug>:
            ${common_compile_opts}
            -O0 -g3 -fno-inline
            -fno-omit-frame-pointer
            -fsanitize=address,undefined,thread,memory,dataflow,leak,type,realtime
            -fstack-protector-strong
            >

            # Release
            $<$<CONFIG:Release>:
            ${common_compile_opts}
            -O3 -DNDEBUG
            -march=native -funroll-loops
            -ffast-math -fomit-frame-pointer
            -flto
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
            ${common_compile_opts}
            -O2 -g
            -march=native -fno-omit-frame-pointer
            -fsanitize=undefined
            >

            # MinSizeRel
            $<$<CONFIG:MinSizeRel>:
            ${common_compile_opts}
            -Os -DNDEBUG
            -fdata-sections -ffunction-sections
            -fmerge-all-constants -flto
            >
    )

    # ------------------ Link Options ------------------
    target_link_options(${target} PRIVATE
            # Debug
            $<$<CONFIG:Debug>:
            -g -fsanitize=address,undefined
            -fuse-ld=lld
            >

            # Release or MinSizeRel
            $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:
            -flto -fuse-ld=lld
            -Wl,--gc-sections
            -Wl,-O2
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
            -g -O2 -fsanitize=undefined
            -fuse-ld=lld
            -Wl,-O1,--as-needed
            >
    )

    # ------------------ Compile Definitions ------------------
    target_compile_definitions(${target} PRIVATE
            # Debug
            $<$<CONFIG:Debug>:
            DEBUG _DEBUG
            ${common_defs}
            >

            # Release
            $<$<CONFIG:Release>:
            NDEBUG
            ${common_defs}
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
            NDEBUG RELWITHDEBINFO
            ${common_defs}
            >

            # MinSizeRel
            $<$<CONFIG:MinSizeRel>:
            NDEBUG MINSIZEREL
            ${common_defs}
            >
    )
endfunction()

function(set_GCC_compile_flags target)
    # --- Common flag sets ---
    set(common_compile_opts
            -Wall -Wextra -Wpedantic -Wconversion -Werror
            -fdiagnostics-color=always
            -fno-omit-frame-pointer
            -lstdc++exp
            -fcontracts
            -std=c++23
    )

    set(common_defs
            _FORTIFY_SOURCE=2
            _FILE_OFFSET_BITS=64
    )

    # ------------------ Compile Options ------------------
    target_compile_options(${target} PRIVATE
            # --- Debug ---
            $<$<CONFIG:Debug>:
            ${common_compile_opts}
            -O0 -g3 -ggdb
            -fstack-protector-strong
            -fno-inline
            -fsanitize=address,undefined,leak,thread,
            -D_GLIBCXX_ASSERTIONS
            >

            # --- Release ---
            $<$<CONFIG:Release>:
            ${common_compile_opts}
            -O3 -DNDEBUG
            -march=native -funroll-loops
            -ffast-math -fomit-frame-pointer
            -flto
            >

            # --- RelWithDebInfo ---
            $<$<CONFIG:RelWithDebInfo>:
            ${common_compile_opts}
            -O2 -g
            -march=native -fno-omit-frame-pointer
            -fsanitize=undefined
            -fno-strict-aliasing
            >

            # --- MinSizeRel ---
            $<$<CONFIG:MinSizeRel>:
            ${common_compile_opts}
            -Os -DNDEBUG
            -fdata-sections -ffunction-sections
            -fmerge-all-constants -flto
            >
    )

    # ------------------ Link Options ------------------
    target_link_options(${target} PRIVATE
            # --- Debug ---
            $<$<CONFIG:Debug>:
            -g -fsanitize=address,undefined
            -fuse-ld=gold
            -Wl,-O0
            >

            # --- Release & MinSizeRel (shared) ---
            $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:
            -flto -fuse-ld=gold
            -Wl,--gc-sections
            -Wl,-O2
            -Wl,--as-needed
            >

            # --- RelWithDebInfo ---
            $<$<CONFIG:RelWithDebInfo>:
            -g -O2 -fsanitize=undefined
            -fuse-ld=gold
            -Wl,-O1,--as-needed
            >
    )

    # ------------------ Compile Definitions ------------------
    target_compile_definitions(${target} PRIVATE
            # --- Debug ---
            $<$<CONFIG:Debug>:
            DEBUG _DEBUG
            ${common_defs}
            >

            # --- Release ---
            $<$<CONFIG:Release>:
            NDEBUG
            ${common_defs}
            >

            # --- RelWithDebInfo ---
            $<$<CONFIG:RelWithDebInfo>:
            NDEBUG RELWITHDEBINFO
            ${common_defs}
            >

            # --- MinSizeRel ---
            $<$<CONFIG:MinSizeRel>:
            NDEBUG MINSIZEREL
            ${common_defs}
            >
    )
endfunction()

