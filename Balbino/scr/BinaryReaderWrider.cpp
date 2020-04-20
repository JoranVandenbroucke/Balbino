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

std::ostream& Balbino::BinaryReadWrite::Write( std::ostream& file, const Balbino::vec3& value )
{
	Write( file, value.x );
	Write( file, value.y );
	Write( file, value.z );
	return file;
}


std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, std::string& value )
{
	int size{};
	Read( file, size );
	if ( size == 0 )return file;
	char* pBuffer{ new char[size] };
	file.read( pBuffer, size );
	value.append( pBuffer, size );
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

std::istream& Balbino::BinaryReadWrite::Read( std::istream& file, Balbino::vec3& value )
{
	Read( file, value.x );
	Read( file, value.y );
	Read( file, value.z );
	return file;
}
