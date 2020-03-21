#include "BalbinoPCH.h"
#include "Struct.h"
using namespace Balbino;

Color::Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
	: r{ r }
	, g{ g }
	, b{ b }
	, a{ a }
{
}

Color::Color( uint32_t color )
	: r{ unsigned char( ( color >> 16 ) & 0xff ) }
	, g{ unsigned char( ( color >> 8 ) & 0xff ) }
	, b{ unsigned char( color & 0xff ) }
	, a{ 255 }
{
}

vec3::vec3( float x, float y, float z )
	: x{ x }
	, y{ y }
	, z{ z }
{
}