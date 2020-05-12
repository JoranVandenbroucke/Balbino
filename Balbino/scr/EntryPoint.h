#pragma once
#include "BalbinoPCH.h"
#include <vld.h>
#ifdef BL_PLATFORM_WINDOWS
#include <iostream>

extern Balbino::Application* Balbino::CreateApplication();
//#undef main

void StartHeapControl();
void DumpMemoryLeaks();

int main( int arc, char* argv[] )
{
	(void) arc;
	(void) argv;
	StartHeapControl();

	auto bubble = Balbino::CreateApplication();
	try
	{
		bubble->Run();
	}
	catch( const std::exception& e )
	{
		std::cout << e.what();
	}
	delete bubble;
	DumpMemoryLeaks();

	return 0;
}

void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, 0 );

	// Report detected leaks when the program exits
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// Set a breakpoint on the specified object allocation order number
	_CrtSetBreakAlloc( 3957 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}
#endif // BL_PLATFORM_WINDOWS