#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H
#include <SDL.h>

#ifdef BL_PLATFORM_WINDOWS
#include <iostream>

extern Balbino::Application* Balbino::CreateApplication();
extern void Balbino::DestroyApplication( Balbino::Application* pApplication );

int main( int arc, char* argv[] )
{
    (void) arc;
    (void) argv;
    
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
#endif // ENTRYPOINT_H
