#pragma once

#include <exception>

#include "Application.h"
#include "Balbino_main.h"

#define SDL_MAIN_NEEDED
#include <SDL3/SDL_main.h>

inline int main( const int argc, char* argv[] )
{
    (void) argc;
    (void) argv;

    auto *const pApp { BalbinoApp::CreateApplication() };
    INIT_MEMORY
    try
    {
        pApp->Initialize();
        pApp->Run();
    }
    catch ( const std::exception& )
    {
        pApp->Cleanup();
        DestroyApplication( pApp );
        DUMP_MEMORY_LEAKS
        return -1;
    }
    pApp->Cleanup();

    DestroyApplication( pApp );
    DUMP_MEMORY_LEAKS

    return 0;
}
