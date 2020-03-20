#include "BalbinoPCH.h"
#include "Struct.h"

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
