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

Balbino::Vector2::Vector2( float x, float y )
	: x{ x }
	, y{ y }
{
}

Vector2& Balbino::Vector2::operator+=( const Vector2& rhs )
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Vector2 Balbino::operator+( Vector2 lhs, const Vector2& rhs )
{
	return lhs += rhs;
}

Vector3::Vector3( float x, float y, float z )
	: x{ x }
	, y{ y }
	, z{ z }
{
}

float Balbino::Vector3::Magnitude()
{
	return std::sqrtf( SqrMagnitude() );
}

Vector3 Balbino::Vector3::Normalized()
{
	float magnitude{ Magnitude() };
	if( magnitude <= 0.001f )
	{
		return PositiveInfinity();
	}
	return Vector3{ x / magnitude, y / magnitude, z / magnitude };
}

float Balbino::Vector3::SqrMagnitude()
{
	return x * x + y * y + z * z;
}

Vector3& Balbino::Vector3::operator+=( const Vector3& rhs )
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}

Vector3& Balbino::Vector3::operator-=( const Vector3& rhs )
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return *this;
}

Vector3& Balbino::Vector3::operator/=( float rhs )
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	return *this;
}

Vector3& Balbino::Vector3::operator*=( float rhs )
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	return *this;
}

Vector3 Balbino::operator+( Vector3 lhs, const Vector3& rhs )
{
	return lhs += rhs;
}

Vector3 Balbino::operator-( Vector3 lhs, const Vector3& rhs )
{
	return lhs -= rhs;
}

Vector3 Balbino::operator/( Vector3 lhs, float rhs )
{
	return lhs /= rhs;
}

Vector3 Balbino::operator*( Vector3 lhs, float rhs )
{
	return lhs *= rhs;
}

bool Balbino::operator!=( Vector3 lhs, const Vector3& rhs )
{
	return !(lhs == rhs);
}

bool Balbino::operator==( Vector3 lhs, const Vector3& rhs )
{
	if( ( lhs.x + 0.001f > rhs.x && lhs.x - 0.001f < rhs.x ) && ( lhs.y + 0.001f > rhs.y && lhs.y - 0.001f < rhs.y ) )
	{
		true;
	}
	return false;
}

Balbino::Vector4::Vector4( float x, float y, float z, float w )
	:x{ x }
	, y{ y }
	, z{ z }
	, w{ w }
{
}

Vector4& Balbino::Vector4::operator+=( const Vector4& rhs )
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->w += rhs.w;
	return *this;
}

Vector4 Balbino::operator+( Vector4 lhs, const Vector4& rhs )
{
	return lhs += rhs;
}

Balbino::Rect::Rect( float xMin, float xMax, float yMin, float yMax )
	:xMin{ xMin }
	, xMax{ xMax }
	, yMin{ yMin }
	, yMax{ yMax }
	, width{ xMax - xMin }
	, height{ yMax - yMin }
{
}

Rect& Balbino::Rect::operator+=( const Rect& rhs )
{
	this->xMin += rhs.xMin;
	this->xMax += rhs.xMax;
	this->yMin += rhs.yMin;
	this->yMax += rhs.yMax;
	return *this;
}

Rect Balbino::operator+( Rect lhs, const Rect& rhs )
{
	return lhs += rhs;
}
