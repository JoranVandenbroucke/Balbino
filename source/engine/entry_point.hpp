#pragma once
#include "balbino_main.hpp"
#include "application.hpp"

import std;


int main(const int /*argc*/, char** /*pArgv*/)
{
    // todo: let's cash/process the input variables for future use

    auto* const pApp{BalbinoApp::CreateApplication()};
    INIT_MEMORY_LEAKS();

    try
    {
        pApp->Initialize();
        pApp->Run();
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
        pApp->Cleanup();
        DestroyApplication(pApp);
        DUMP_MEMORY_LEAKS();
        return -1;
    }
    pApp->Cleanup();

    DestroyApplication(pApp);
    DUMP_MEMORY_LEAKS();

    return 0;
}
