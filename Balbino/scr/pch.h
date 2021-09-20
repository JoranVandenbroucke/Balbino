#pragma once
#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#define DBG_NEW new( _NORMAL_BLOCK ,  __FILE__ ,  __LINE__ )
#else
#define DBG_NEW new
#endif // _DEBUG

// SDL libs
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")

// OpenGL libs
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")

// SDL extension libs 
#pragma comment(lib, "SDL2_image.lib")  
#pragma comment(lib, "SDL2_ttf.lib") 
#pragma comment(lib, "SDL2_mixer.lib")  

// SDL and OpenGL Includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h> 
#include <SDL_opengl.h>
#include <SDL_ttf.h> 

#include <gl/GLU.h>

#include <algorithm>
#include <cstdio>
#include <iostream> // std::cout
#include <memory> // smart pointers
#include <sstream> // stringstream
#include <string>
#include <vector>

//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
