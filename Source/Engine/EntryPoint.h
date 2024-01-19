#pragma once
#include <iostream>

#include "Application.h"
#include "Balbino_main.h"

#define SDL_MAIN_NEEDED
#include <SDL3/SDL_main.h>

int main( const int argc, char* argv[] )
{
    (void) argc;
    (void) argv;

    const auto pApp { BalbinoApp::CreateApplication() };
    INIT_MEMORY
    try
    {
        pApp->Initialize();
        pApp->Run();
    }
    catch ( const std::exception& e )
    {
        std::cout << e.what() << std::endl;
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
