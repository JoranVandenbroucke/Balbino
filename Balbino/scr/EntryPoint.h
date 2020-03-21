#pragma once
#ifdef BL_PLATFORM_WINDOWS
#include <iostream>

extern Balbino::Application* Balbino::CreateApplication();

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
