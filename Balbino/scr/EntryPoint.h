#pragma once
#ifdef BL_PLATFORM_WINDOWS
#include <vld.h>
#include <iostream>

extern Balbino::Application* Balbino::CreateApplication();
//#undef main

int main( int arc, char* argv[] )
{
	auto bubble = Balbino::CreateApplication();
	try
	{
		bubble->Run();
	}
	catch( const std::exception& e)
	{
		std::cout << e.what();
	}
	delete bubble;
	return 0;
}
#endif // BL_PLATFORM_WINDOWS
