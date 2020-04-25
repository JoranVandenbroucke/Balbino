#pragma once
#include <cstdint>

namespace Balbino
{

	struct Color
	{
		Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255 );
		Color( uint32_t color );

		unsigned char r, g, b, a;
	};

	struct Vector2
	{
		Vector2( float x = 0.f, float y = 0.f );
		float x, y;

		Vector2& operator+=( const Vector2& rhs );
		friend Vector2 operator+( Vector2 lhs, const Vector2& rhs );
	};

	struct Vector3
	{
		Vector3( float x = 0.f, float y = 0.f, float z = 0.f );
		float x, y, z;

		Vector3& operator+=( const Vector3& rhs );
		friend Vector3 operator+( Vector3 lhs, const Vector3& rhs );
	};

	struct Vector4
	{
		Vector4( float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f );
		float x, y, z, w;

		Vector4& operator+=( const Vector4& rhs );
		friend Vector4 operator+( Vector4 lhs, const Vector4& rhs );
	};

	struct Rect
	{
		Rect( float xMin = 0.f, float xMax = 0.f, float yMin = 0.f, float yMax = 0.f );
		union
		{
			float xMin;
			float left;
		};
		union
		{
			float xMax;
			float right;
		};
		union
		{
			float yMin;
			float top;
		};
		union
		{
			float yMax;
			float bottom;
		};
		float width, height;

		Rect& operator+=( const Rect& rhs );
		friend Rect operator+( Rect lhs, const Rect& rhs );
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