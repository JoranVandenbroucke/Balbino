#pragma once
#include "Balbino_main.h"
#include <Core.h>
#include <SDL3/SDL.h>
#include <iostream>

#include "Application.h"

int BL_MAIN( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;

    auto pApp = BalbinoApp::CreateApplication();
    INIT_MEMORY
    try
    {
        pApp->Initialize();
        pApp->Run();
    }
    catch ( const std::exception& e )
    {
        std::cout << e.what();
        pApp->Cleanup();
        BalbinoApp::DestroyApplication( pApp );
        DUMP_MEMORY_LEAKS

        return -1;
    }
    pApp->Cleanup();

    BalbinoApp::DestroyApplication( pApp );
    DUMP_MEMORY_LEAKS

    return 0;
}