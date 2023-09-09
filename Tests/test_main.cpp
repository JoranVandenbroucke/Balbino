//
// Created by joran on 31/07/2023.
//
#include "AntlerTestCore.h"

int main( int argc, char** argv )
{
    StartupFunctionManager::GetInstance().CallAllStartupFunctions();
    int returnV = Catch::Session().run( argc, argv );
    StartupFunctionManager::GetInstance().CallAllShutdownFunctions();
    return returnV;
}