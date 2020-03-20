#pragma once
#include <cstdint>
struct Color
{
	Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255 );
	Color( uint32_t color );

	unsigned char r, g, b, a;
};