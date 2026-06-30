//
// Created by joran on 03/09/2023.
//

#pragma once

// ----- Windows + MSVC (incl. clang-cl) -----
#if defined(_WIN32) && defined(_MSC_VER)

#    define _CRTDBG_MAP_ALLOC
#    include <crtdbg.h>

#    define INIT_MEMORY_LEAKS()                                                                                                                                                    \
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);                                                                                                              \
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);                                                                                                                          \
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);                                                                                                                         \
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG)

#    define DUMP_MEMORY_LEAKS()                                                                                                                                                    \
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);                                                                                                                          \
        _CrtDumpMemoryLeaks()

// ----- Linux + GCC/Clang with LeakSanitizer active at compile time -----
// __has_feature is Clang; __SANITIZE_ADDRESS__ covers GCC -fsanitize=address.
// -fsanitize=leak alone sets __SANITIZE_LEAK__ on GCC 13+ / Clang 17+,
// so we check all three to be safe.
// LSan's runtime interface is only available on Linux (not Windows/macOS),
// so this branch is additionally gated on __linux__.
#elif defined(__linux__) && (defined(__GNUC__) || defined(__clang__))                                                                                                              \
    && ((defined(__has_feature) && (__has_feature(address_sanitizer) || __has_feature(leak_sanitizer))) || defined(__SANITIZE_ADDRESS__) || defined(__SANITIZE_LEAK__))

#    include <sanitizer/lsan_interface.h>

#    define INIT_MEMORY_LEAKS() ((void)0) // runtime auto-initialises
#    define DUMP_MEMORY_LEAKS() __lsan_do_recoverable_leak_check()

#else

// ----- Fallback: no-ops (release builds / unsupported platform-compiler combo / sanitizer not enabled) -----
#    define INIT_MEMORY_LEAKS() ((void)0)
#    define DUMP_MEMORY_LEAKS() ((void)0)

#endif
