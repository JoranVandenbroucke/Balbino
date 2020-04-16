#include "BalbinoPCH.h"
#include "BinaryReaderWrider.h"

std::ostream& BinaryReadWrite::Write( std::ostream& file, const std::string& value )
{
	const char* pText = value.c_str();
	const int size = int( value.size() );

	Write( file, size );
	return file.write( pText, size );
}


std::istream& BinaryReadWrite::Read( std::istream& file, std::string& value )
{
	int size{};
	Read( file, size );
	if ( size == 0 )return file;
	char* pBuffer{ new char[size] };
	file.read( pBuffer, size );
	value.append( pBuffer, size );
	return file;
}