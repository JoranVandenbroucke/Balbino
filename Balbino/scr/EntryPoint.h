#pragma once

#ifdef BL_PLATFORM_WINDOWS

extern Balbino::Application* Balbino::CreateApplication();

int main(int arc, char* argv[])
{
	auto bubble = Balbino::CreateApplication();
	bubble->Run();
	delete bubble;
	return 0;
}
#endif // BL_PLATFORM_WINDOWS
