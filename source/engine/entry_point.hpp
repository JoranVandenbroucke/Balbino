#pragma once

#include <exception>

#include "application.hpp"
#include "balbino_main.h"

#define SDL_MAIN_NEEDED
#include <SDL3/SDL_main.h>

#if defined(_MSC_VER)

// ----- MSVC / Windows -----
#    define _CRTDBG_MAP_ALLOC
#    include <crtdbg.h>
#    include <stdlib.h>

#    define INIT_MEMORY_LEAKS()                                                \
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);          \
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);                      \
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);                     \
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG)

#    define DUMP_MEMORY_LEAKS()                                                \
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);                      \
        _CrtDumpMemoryLeaks()

#elif defined(__has_include) && __has_include(<sanitizer/lsan_interface.h>)

// ----- GCC / Clang with LeakSanitizer -----
// Compile with: -fsanitize=address,leak  (or -fsanitize=leak)
#    include <sanitizer/lsan_interface.h>

#    define INIT_MEMORY_LEAKS()                                                \
        /* LSan is initialised automatically by the sanitizer runtime */       \
        ((void)0)

#    define DUMP_MEMORY_LEAKS() __lsan_do_recoverable_leak_check()

#else

// ----- Fallback: no-ops (release builds / unsupported toolchain) -----
#    define INIT_MEMORY_LEAKS()
#    define DUMP_MEMORY_LEAKS()
#endif

inline int main([[maybe_unused]] const int argc, [[maybe_unused]] char** pArgv)
{
    // todo: let's cash/process the input variables for future use

    auto* const pApp{BalbinoApp::CreateApplication()};
    INIT_MEMORY_LEAKS();

    try
    {
        pApp->Initialize();
        pApp->Run();
    }
    catch (const std::exception&)
    {
        pApp->Cleanup();
        DestroyApplication(pApp);
        DUMP_MEMORY_LEAKS()
        return -1;
    }
    pApp->Cleanup();

    DestroyApplication(pApp);
    DUMP_MEMORY_LEAKS()

    return 0;
}
