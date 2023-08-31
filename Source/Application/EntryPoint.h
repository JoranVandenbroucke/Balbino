#pragma once
#include <Core.h>

#include <SDL3/SDL.h>
#include <iostream>

#ifdef BL_PLATFORM_WINDOWS
#include "Application.h"

#ifdef _DEBUG
int main( int arc, char* argv[] )
#else
int WinMain( int arc, char* argv[] )
#endif
{
    (void)arc;
    (void)argv;

    auto pApp = BalbinoApp::CreateApplication();

#ifdef _DEBUG
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
#endif
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
#ifdef _DEBUG
        _CrtDumpMemoryLeaks();
#endif
        return -1;
    }
    pApp->Cleanup();

    BalbinoApp::DestroyApplication( pApp );
#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}

#endif// BL_PLATFORM_WINDOWS
