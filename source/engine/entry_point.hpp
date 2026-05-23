#pragma once
import std;


#include "application.hpp"
#include "balbino_main.hpp"

int main([[maybe_unused]] const int argc, [[maybe_unused]] char** pArgv)
{
    // todo: let's cash/process the input variables for future use

    auto* const pApp{BalbinoApp::CreateApplication()};
    INIT_MEMORY_LEAKS();

    try
    {
        std::cout << "initialize start";
        pApp->Initialize();
        std::cout << "initialize end";
        std::cout <<"run start\n";
        pApp->Run();
        std::cout <<"run end\n";
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what();
        pApp->Cleanup();
        DestroyApplication(pApp);
        DUMP_MEMORY_LEAKS();
        return -1;
    }
    std::cout <<"cleanup start\n";
    pApp->Cleanup();
    std::cout <<"cleanup end\n";

    DestroyApplication(pApp);
    DUMP_MEMORY_LEAKS();

    return 0;
}
