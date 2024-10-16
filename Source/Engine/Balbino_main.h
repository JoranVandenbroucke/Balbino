//
// Created by joran on 03/09/2023.
//

#pragma once
#include "Versions.hpp"

#define INIT_MEMORY
#define DUMP_MEMORY_LEAKS

#if defined( __MINGW32__ ) || defined( _WIN32 )// Check if compiling for Windows
    #ifdef BALBINO_DEBUG
        #undef INIT_MEMORY
        #undef DUMP_MEMORY_LEAKS
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
        #define INIT_MEMORY _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
        #define DUMP_MEMORY_LEAKS _CrtDumpMemoryLeaks();
    #endif
#endif
