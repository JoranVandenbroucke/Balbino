//
// Created by joran on 03/09/2023.
//

#pragma once

#define BL_MAIN main
#define INIT_MEMORY
#define DUMP_MEMORY_LEAKS

#if defined( __MINGW32__ ) || defined( _WIN32 )// Check if compiling for Windows

#ifdef _DEBUG
#undef INIT_MEMORY
#undef DUMP_MEMORY_LEAKS
#define INIT_MEMORY _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
#define DUMP_MEMORY_LEAKS _CrtDumpMemoryLeaks();
#endif

#if !defined( BALBINO_EDITOR )
#undef BL_MAIN
#define BL_MAIN WinMain
#endif

#endif
