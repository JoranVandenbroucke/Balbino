#pragma once

#include <cstdint>
#if defined( __MINGW32__ ) || defined( _WIN32 )// Check if compiling for Windows
#ifndef BL_PLATFORM_WINDOWS
#define BL_PLATFORM_WINDOWS
#endif
#ifndef NOMINMAX
#define NOMINMAX// Prevent Windows from defining min and max macros
#endif
#define BALBINO_WINDOWS_INCLUDE
#ifdef BL_BUILD_DLL
#define BALBINO_API __declspec( dllexport )
#elif BL_BUILD_LIB
#define BALBINO_API
#else
#define BALBINO_API __declspec( dllimport )
#endif                    // BL_BUILD_DLL
#elif defined( __linux__ )// Check if compiling for Linux
#define BL_PLATFORM_LINUX
#define BALBINO_LINUX_INCLUDE
#ifdef BL_BUILD_DLL
#define BALBINO_API __attribute__( ( visibility( "default" ) ) )
#elif BL_BUILD_LIB
#define BALBINO_API
#else
#define BALBINO_API
#endif// BL_BUILD_DLL
#else
#error "Unsupported platform! BALBINO works only on Windows and Linux."
#endif

// Include platform-specific headers
#if defined( BALBINO_WINDOWS_INCLUDE )
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#endif
#endif

#if defined( BALBINO_LINUX_INCLUDE )
// Include Linux-specific headers here
#endif

#ifndef BALBINO_NULL
#define BALBINO_NULL nullptr
#endif

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "ToCompilerConsole.h"

consteval uint8_t operator"" _uc( unsigned long long arg ) noexcept
{
    return static_cast<uint8_t>( arg );
}
consteval int8_t operator"" _c( unsigned long long arg ) noexcept
{
    return static_cast<int8_t>( arg );
}

struct StringHash final {
    using is_transparent = void;

    inline std::size_t operator()( const std::string& str ) const
    {
        return std::hash<std::string_view> {}( str );
    }

    inline std::size_t operator()( std::string_view str ) const
    {
        return std::hash<std::string_view> {}( str );
    }
};
