#pragma once

#ifdef BL_PLATFORM_WINDOWS
	#ifdef BL_BUILD_DLL
		#define BALBINO_API __declspec(dllexport)
	#elif BL_BUILD_LIB
			#define BALBINO_API
	#else
			#define BALBINO_API __declspec(dllimport)
	#endif // BL_BUILD_DLL
#else
	#error BALBINO WORKS ONLY ON WINDOWS
#endif // BL_PLATFORM_WINDOWS
