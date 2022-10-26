#pragma once

#ifdef BL_PLATFORM_WINDOWS
#include <iostream>
//#include <vld.h>

extern Balbino::Application* Balbino::CreateApplication();
extern void Balbino::DestroyApplication( Balbino::Application* pApplication );
#if _DEBUG
int main( int arc, char* argv[] )
#else
int WinMain( int arc, char* argv[] )
#endif
{
    (void) arc;
    (void) argv;
    //_CrtSetBreakAlloc( 392 );
    auto pApp = Balbino::CreateApplication();
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
        return -1;
    }
    pApp->Cleanup();
    
    Balbino::DestroyApplication( pApp );
    return 0;
}
#endif // BL_PLATFORM_WINDOWS
