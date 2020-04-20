#pragma once
#include <cstdint>
#include "Core.h"

namespace Balbino
{

	struct BALBINO_API Color
	{
		Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255 );
		Color( uint32_t color );

		unsigned char r, g, b, a;
	};

	struct BALBINO_API vec3
	{
		vec3( float x = 0.f, float y = 0.f, float z = 0.f );
		float x, y, z;

		vec3& operator+=( const vec3& rhs );
		friend vec3 operator+( vec3 lhs, const vec3& rhs );
	};

	enum class ComponentList
	{
		Audio,
		LoggedAudio,
		Avatar,
		Camera,
		FPSScript,
		Text,
		Texture2D,
		Transform
	};
}