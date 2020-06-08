#include "BalbinoPCH.h"
#include "BinaryReaderWrider.h"

std::ostream& Balbino::BinaryReadWrite::Write( std::ostream& file, const std::string& value )
{
	const char* pText = value.c_str();
	const int size = int( value.size() );

	Write( file, size );
	return file.write( pText, size );
}

std::ostream& Balbino::BinaryReadWrite::Write( std::ostream& file, const Balbino::Color& value )
{
	Write( file, value.r );
	Write( file, value.g );
	Write( file, value.b );
	Write( file, value.a );
	return file;
}

std::ostream& Balbino::BinaryReadWrite::Write( std::ostream& file, const Balbino::Vector2& value )
{
	Write( file, value.x );
	Write( file, value.y );
	return file;
}

std::ostream& Balbino::BinaryReadWrite::Write( std::ostream& file, const Balbino::Vector3& value )
{
	Write( file, value.x );
	Write( file, value.y );
	Write( file, value.z );
	return file;
}

std::ostream& Balbino::BinaryReadWrite::Write( std::ostream& file, const Balbino::Vector4& value )
{
	Write( file, value.x );
	Write( file, value.y );
	Write( file, value.z );
	Write( file, value.w );
	return file;
}


std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, std::string& value )
{
	value.clear();
	int size{};
	Read( file, size );
	if ( size == 0 )return file;
	char* pBuffer{ DBG_NEW char[size] };
	file.read( pBuffer, size );
	value.append( pBuffer, size );
	delete[] pBuffer;
	return file;
}

std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, Balbino::Color& value )
{
	Read( file, value.r );
	Read( file, value.g );
	Read( file, value.b );
	Read( file, value.a );
	return file;
}

std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, Balbino::Vector2& value )
{
	Read( file, value.x );
	Read( file, value.y );
	return file;
}

std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, Balbino::Vector3& value )
{
	Read( file, value.x );
	Read( file, value.y );
	Read( file, value.z );
	return file;
}

std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, Balbino::Vector4& value )
{
	Read( file, value.x );
	Read( file, value.y );
	Read( file, value.z );
	Read( file, value.w );
	return file;
}
