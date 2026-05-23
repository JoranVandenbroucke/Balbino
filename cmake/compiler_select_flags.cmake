# set_compile_flags(target)
#
# Unified compile/link/define setup for MSVC, Clang, and GCC.
# Configs:
#   Debug        — max debug info, all sanitizers, no optimization
#   RelWithDebInfo — optimized but debuggable, UBSan
#   Release      — maximum speed, LTO, no safety nets
#   MinSizeRel   — minimum binary size, LTO, stripped
function(set_compile_flags target)

    # -------------------------------------------------------------------------
    # Detect compiler once
    # -------------------------------------------------------------------------
    set(IS_MSVC  "$<CXX_COMPILER_ID:MSVC>")
    set(IS_CLANG "$<CXX_COMPILER_ID:Clang,AppleClang>")
    set(IS_GCC   "$<CXX_COMPILER_ID:GNU>")

    # Config shorthands
    set(CFG_DBG  "$<CONFIG:Debug>")
    set(CFG_RWD  "$<CONFIG:RelWithDebInfo>")
    set(CFG_REL  "$<CONFIG:Release>")
    set(CFG_MSR  "$<CONFIG:MinSizeRel>")

    # Composite guards: compiler + config
    set(MSVC_DBG  "$<AND:${IS_MSVC},${CFG_DBG}>")
    set(MSVC_RWD  "$<AND:${IS_MSVC},${CFG_RWD}>")
    set(MSVC_REL  "$<AND:${IS_MSVC},${CFG_REL}>")
    set(MSVC_MSR  "$<AND:${IS_MSVC},${CFG_MSR}>")

    set(GNU_DBG   "$<AND:${IS_GCC},${CFG_DBG}>")
    set(GNU_RWD   "$<AND:${IS_GCC},${CFG_RWD}>")
    set(GNU_REL   "$<AND:${IS_GCC},${CFG_REL}>")
    set(GNU_MSR   "$<AND:${IS_GCC},${CFG_MSR}>")

    set(CLG_DBG   "$<AND:${IS_CLANG},${CFG_DBG}>")
    set(CLG_RWD   "$<AND:${IS_CLANG},${CFG_RWD}>")
    set(CLG_REL   "$<AND:${IS_CLANG},${CFG_REL}>")
    set(CLG_MSR   "$<AND:${IS_CLANG},${CFG_MSR}>")

    # =========================================================================
    # COMPILE OPTIONS
    # =========================================================================
    target_compile_options(${target} PRIVATE

        # ── MSVC ──────────────────────────────────────────────────────────────
        $<${IS_MSVC}:
            /W4 /WX /MP /permissive-
            /Zc:__cplusplus /Zc:inline
            /wd4996  # suppress CRT deprecation; we define _CRT_SECURE_NO_WARNINGS anyway
        >

        # Debug: full symbols, no opts, ASAN, RTC
        $<${MSVC_DBG}:  /Od /Zi /RTC1 /fsanitize=address /DEBUG:FULL>

        # RelWithDebInfo: O2 + full PDB, UBSan not available on MSVC so keep GS
        $<${MSVC_RWD}:  /O2 /Oi /Zi /DEBUG:FULL /GS /EHsc /fp:fast /Gy>

        # Release: max speed — vectoriser, whole-program, no safety overhead
        $<${MSVC_REL}:
            /O2 /Oi /Ot /Ox       # full optimisation + favour speed
            /GL                    # whole-program optimisation (pairs with /LTCG)
            /Gy /Gw                # function-level & data-level linking
            /Qpar                  # auto-paralleliser
            /fp:fast               # relaxed FP
            /GS-                   # no buffer security check
            /arch:AVX2             # assume AVX2; change to SSE2/AVX if needed
            /favor:blend
            /EHs-c-                # no exceptions
            /GR-                   # no RTTI
        >

        # MinSizeRel: shrink everything
        $<${MSVC_MSR}:
            /O1 /Os /Oy /Gy /Gw /GL
            /GS- /GR- /EHs-c-
            /fp:precise            # precise is smaller code than fast here
            /arch:SSE2
            /Oi-
            /Zc:threadSafeInit-    # removes guard variables
        >

        # ── GCC / Clang shared ────────────────────────────────────────────────
        $<$<OR:${IS_GCC},${IS_CLANG}>:
            -Wall -Wextra -Wpedantic -Wconversion -Werror
            -fno-omit-frame-pointer  # overridden per-config below when safe
            -fvisibility=hidden
            -fvisibility-inlines-hidden  # also hide inline functions in headers
        >

        # ── Clang-specific baseline ───────────────────────────────────────────
        $<${IS_CLANG}:
            -fcolor-diagnostics -fansi-escape-codes
        >

        # ── GCC-specific baseline ─────────────────────────────────────────────
        $<${IS_GCC}:
            -fdiagnostics-color=always
        >

        # ── GCC/Clang Debug ───────────────────────────────────────────────────
        # Full symbols, every sanitizer, no inlining
        $<$<OR:${GNU_DBG},${CLG_DBG}>:
            -O0 -fno-inline -g
            -fstack-protector-strong
            -fsanitize=address,leak
            -fno-omit-frame-pointer
        >

        # GCC Debug extras (thread + ggdb)
        $<${GNU_DBG}: -ggdb -D_GLIBCXX_ASSERTIONS>

        # ── GCC/Clang RelWithDebInfo ──────────────────────────────────────────
        # Optimised but still debuggable; UBSan only (ASAN+debug is expensive)
        $<$<OR:${GNU_RWD},${CLG_RWD}>:
            -O2 -g
            -march=native
            -fno-omit-frame-pointer
            -fsanitize=undefined
            -fno-strict-aliasing
        >

        # ── GCC/Clang Release ─────────────────────────────────────────────────
        # Every speed lever: native tuning, LTO, fast-math, vectoriser hints
        $<$<OR:${GNU_REL},${CLG_REL}>:
            -O3
            -march=native -mtune=native
            -funroll-loops
            -ffast-math
            -fomit-frame-pointer
            -flto=auto               # thin-LTO on Clang, fat-LTO on GCC
            -fno-stack-protector
            -fno-exceptions
            -fno-rtti
        >

        # ── GCC/Clang MinSizeRel ──────────────────────────────────────────────
        $<$<OR:${GNU_MSR},${CLG_MSR}>:
            -Os
            -fdata-sections -ffunction-sections
            -fmerge-all-constants
            -flto=auto
            -fno-exceptions
            -fno-rtti
            -fno-unwind-tables
            -fno-asynchronous-unwind-tables
        >
    )

    # =========================================================================
    # LINK OPTIONS
    # =========================================================================
    target_link_options(${target} PRIVATE

        # ── MSVC Debug ────────────────────────────────────────────────────────
        $<${MSVC_DBG}:
            /DEBUG:FULL /INCREMENTAL
            /OPT:NOREF /OPT:NOICF
            /PDBALTPATH:%_PDB%
        >

        # ── MSVC RelWithDebInfo ───────────────────────────────────────────────
        $<${MSVC_RWD}:
            /DEBUG:FULL /INCREMENTAL
            /OPT:NOREF /OPT:NOICF
        >

        # ── MSVC Release ──────────────────────────────────────────────────────
        $<${MSVC_REL}:
            /LTCG /INCREMENTAL:NO
            /OPT:REF /OPT:ICF=5     # 5 iterations of ICF
            /NODEFAULTLIB:msvcrtd
        >

        # ── MSVC MinSizeRel ───────────────────────────────────────────────────
        $<${MSVC_MSR}:
            /LTCG /INCREMENTAL:NO
            /OPT:REF /OPT:ICF=5
            /MERGE:.rdata=.text /MERGE:.data=.text
            /NODEFAULTLIB:msvcrtd
            /RELEASE
        >

        # ── GCC Debug ─────────────────────────────────────────────────────────
        $<${GNU_DBG}:
            -g -fsanitize=address,undefined,leak
            -fuse-ld=gold
        >

        # ── GCC RelWithDebInfo ────────────────────────────────────────────────
        $<${GNU_RWD}:
            -g -fsanitize=undefined
            -fuse-ld=gold
            -Wl,-O1,--as-needed
        >

        # ── GCC Release & MinSizeRel ──────────────────────────────────────────
        $<$<OR:${GNU_REL},${GNU_MSR}>:
            -flto=auto
            -fuse-ld=gold
            -Wl,--gc-sections -Wl,-O2 -Wl,--as-needed
        >

        # ── Clang Debug ───────────────────────────────────────────────────────
        $<${CLG_DBG}:
            -g -fsanitize=address,undefined,leak
            -fuse-ld=lld
        >

        # ── Clang RelWithDebInfo ──────────────────────────────────────────────
        $<${CLG_RWD}:
            -g -fsanitize=undefined
            -fuse-ld=lld
            -Wl,-O1,--as-needed
        >

        # ── Clang Release & MinSizeRel ────────────────────────────────────────
        $<$<OR:${CLG_REL},${CLG_MSR}>:
            -flto=thin              # ThinLTO is Clang's sweet spot
            -fuse-ld=lld
            -Wl,--gc-sections -Wl,-O2 -Wl,--as-needed
        >
    )

    # =========================================================================
    # COMPILE DEFINITIONS
    # =========================================================================
    target_compile_definitions(${target} PRIVATE

        # ── Cross-compiler always-on ──────────────────────────────────────────
        $<${IS_MSVC}:
            _CRT_SECURE_NO_WARNINGS
            _SCL_SECURE_NO_WARNINGS
            _ENABLE_EXTENDED_ALIGNED_STORAGE=1
        >

        # ── Debug (all compilers) ─────────────────────────────────────────────
        $<${CFG_DBG}: DEBUG _DEBUG>
        $<${MSVC_DBG}: _ITERATOR_DEBUG_LEVEL=2 _CRTDBG_MAP_ALLOC>
        $<${GNU_DBG}:  _GLIBCXX_DEBUG>

        # ── RelWithDebInfo ────────────────────────────────────────────────────
        $<${CFG_RWD}: RELWITHDEBINFO>
        $<${MSVC_RWD}: _ITERATOR_DEBUG_LEVEL=1 _SECURE_SCL=1 _HAS_ITERATOR_DEBUGGING=1>

        # ── Release ───────────────────────────────────────────────────────────
        $<${CFG_REL}: NDEBUG>
        $<${MSVC_REL}:
            _SECURE_SCL=0
            _HAS_ITERATOR_DEBUGGING=0
            _ITERATOR_DEBUG_LEVEL=0
            _HAS_EXCEPTIONS=0
            _HAS_RTTI=0
        >

        # ── MinSizeRel ────────────────────────────────────────────────────────
        $<${CFG_MSR}: NDEBUG MINSIZEREL>
        $<${MSVC_MSR}:
            _SECURE_SCL=0
            _HAS_ITERATOR_DEBUGGING=0
            _ITERATOR_DEBUG_LEVEL=0
            _HAS_EXCEPTIONS=0
            _HAS_RTTI=0
            _STATIC_CPPLIB
            _NO_CRT_STDIO_INLINE
        >
    )

endfunction()
