#ifndef BALBINO_ENTRYPOINT_H
#define BALBINO_ENTRYPOINT_H

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <SDL3/SDL.h>

#ifdef BL_PLATFORM_WINDOWS
#include <iostream>
#include "Application.h"

int main( int arc, char* argv[] )
{
    (void) arc;
    (void) argv;
    
    auto pApp = Balbino::CreateApplication();
    
    #ifdef _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
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
        Balbino::DestroyApplication( pApp );
        #ifdef _DEBUG
        _CrtDumpMemoryLeaks();
        #endif
        return -1;
    }
    pApp->Cleanup();
    
    Balbino::DestroyApplication( pApp );
    #ifdef _DEBUG
    _CrtDumpMemoryLeaks();
    #endif
    return 0;
}

#endif // BL_PLATFORM_WINDOWS
#endif // BALBINO_ENTRYPOINT_H
