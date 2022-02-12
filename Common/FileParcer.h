#pragma once
#include <array>
#include <deque>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Shader.h"
#include "UUID.h"

enum class EFileTypes : uint8_t
{
	Unknown,
	Folder,
	Scene,
	Image,
	Audio,
	Font,
	Model,
	Shader,
	Material,
	Code,
	Preset
};

inline const char* ToString( EFileTypes type )
{
	switch ( type )
	{
		case EFileTypes::Unknown:
			return "Unknown";
		case EFileTypes::Folder:
			return "Folder";
		case EFileTypes::Scene:
			return "Scene";
		case EFileTypes::Code:
			return "Code";
		case EFileTypes::Image:
			return "Image";
		case EFileTypes::Audio:
			return "Audio";
		case EFileTypes::Font:
			return "Font";
		case EFileTypes::Model:
			return "Model";
		case EFileTypes::Preset:
			return "Preset";
		case EFileTypes::Shader:
			return "Shader";
		case EFileTypes::Material:
			return "Material";
	}
	return "";
}

struct SFile
{
	bool isFolder;
	EFileTypes type;
	uint64_t uuid;
	uint64_t size;
	void* pData;
	std::string fileName;
	std::filesystem::path path;
	std::filesystem::file_time_type lastWrittenTime;
	int depth;

	bool operator==( const uint64_t& id ) const
	{
		return id == uuid;
	}
	bool operator==( const CUuid& id ) const
	{
		return ( uint64_t ) id == uuid;
	}
	bool operator==(const std::string& otherName) const
	{
		return otherName == fileName;
	}
	bool operator==( const std::filesystem::path& otherPath ) const
	{
		return otherPath == path;
	}
	bool operator==(const SFile& other) const
	{
		return this->uuid == other.uuid;
	}
};

namespace BinaryReadWrite
{
	//write
	template <typename T>
	std::ostream& Write( std::ostream& file, const T& value );
	template <typename T>
	std::ostream& Write( std::ostream& file, const T* pValue, uint64_t size );
	template <typename T, uint64_t SIZE>
	std::ostream& Write( std::ostream& file, const std::array<T, SIZE>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::vector<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::deque<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::forward_list<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::list<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::stack<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::queue<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::priority_queue<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::set<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::multiset<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::unordered_set<T>& values );
	template <typename T>
	std::ostream& Write( std::ostream& file, const std::unordered_multiset<T>& values );
	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::map<K, V>& values );
	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::multimap<K, V>& values );
	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::unordered_map<K, V>& values );
	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::unordered_multimap<K, V>& values );
	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::pair<K, V>& values );

	std::ostream& Write( std::ostream& file, const std::string& value );
	std::ostream& Write( std::ostream& file, const BalVulkan::SShaderResource& value );

	//read
	template <typename T>
	std::istream& Read( std::istream& file, T& value );
	template <typename T>
	std::istream& Read( std::istream& file, T*& pValue, uint64_t size );
	template <typename T, uint64_t SIZE>
	std::istream& Read( std::istream& file, std::array<T, SIZE>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::vector<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::deque<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::forward_list<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::list<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::stack<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::queue<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::priority_queue<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::set<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::multiset<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::unordered_set<T>& values );
	template <typename T>
	std::istream& Read( std::istream& file, std::unordered_multiset<T>& values );
	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::map<K, V>& values );
	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::multimap<K, V>& values );
	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::unordered_map<K, V>& values );
	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::unordered_multimap<K, V>& values );
	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::pair<K, V>& values );

	std::istream& Read( std::istream& file, std::string& value );
	std::istream& Read( std::istream& file, BalVulkan::SShaderResource& value );
	/////////////////////////////////////////////////////////////////////////////////////////////
	std::istream& IsAtStart( std::istream& file, bool& isAtStart );
	std::istream& IsAtEnd( std::istream& file, bool& isAtEnd );
	std::istream& MoveCursor( std::istream& file, int value );
	std::istream& MoveCursorTo( std::istream& file, int value );
	std::istream& MoveCursorToStart( std::istream& file );
	std::istream& MoveCursorToEnd( std::istream& file );
	std::istream& GetCursorPosition( std::istream& file, uint64_t& size );
	std::istream& GetData( std::istream& file, char* pData, int64_t size );

	template <typename T>
	std::ostream& Write( std::ostream& file, const T& value )
	{
		if ( ( file.rdstate() & std::ifstream::failbit ) != 0 )
		{
			std::cerr << "Error writing\n";
			return file;
		}
		if ( !std::is_standard_layout_v<T> )
		{
			return file;
		}
		return file.write( reinterpret_cast< const char* >( &value ), sizeof( T ) );
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const T* pValue, uint64_t size )
	{
		return file.write( reinterpret_cast< const char* >( pValue ), size );
	}

	template <typename T, uint64_t SIZE>
	std::ostream& Write( std::ostream& file, const std::array<T, SIZE>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Write( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::vector<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Write( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::deque<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Write( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::forward_list<T>& values )
	{
		const int size{ static_cast< int >( std::distance( values.begin(), values.end() ) ) };
		Write( file, size );
		for ( const T& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::list<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Write( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::stack<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Write( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::queue<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		std::queue<T> copy{ values };
		while ( !copy.empty() )
		{
			Write( file, copy.front() );
			copy.pop();
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::priority_queue<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		std::priority_queue<T> copy{ values };
		while ( !copy.empty() )
		{
			Write( file, copy.top() );
			copy.pop();
		}

		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::set<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const T& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::multiset<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const T& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::unordered_set<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const T& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename T>
	std::ostream& Write( std::ostream& file, const std::unordered_multiset<T>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const T& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::map<K, V>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const std::pair<K, V>& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::multimap<K, V>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const std::pair<K, V>& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::unordered_map<K, V>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const std::pair<K, V>& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::unordered_multimap<K, V>& values )
	{
		const int size{ static_cast< int >( values.size() ) };
		Write( file, size );
		for ( const std::pair<K, V>& value : values )
		{
			Write( file, value );
		}
		return file;
	}

	template <typename K, typename V>
	std::ostream& Write( std::ostream& file, const std::pair<K, V>& values )
	{
		Write( file, values.first );
		Write( file, values.second );
		return file;
	}

	//read
	template <typename T>
	std::istream& Read( std::istream& file, T& value )
	{
		if ( ( file.rdstate() & std::ifstream::failbit ) != 0 )
		{
			std::cerr << "Error reading\n";
			return file;
		}
		if ( !std::is_standard_layout_v<T> )
		{
			return file;
		}
		return file.read( reinterpret_cast< char* >( &value ), sizeof( T ) );
	}

	template <typename T>
	std::istream& Read( std::istream& file, T*& pValue, uint64_t size )
	{
		return file.read( reinterpret_cast< char* >( pValue ), size );
	}

	template <typename T, uint64_t SIZE>
	std::istream& Read( std::istream& file, std::array<T, SIZE>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Read( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::vector<T>& values )
	{
		int size{ static_cast< int >( values.size() ) };
		Read( file, size );
		values.resize( size );
		for ( int i = 0; i < size; i++ )
		{
			Read( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::deque<T>& values )
	{
		int size{};
		Read( file, size );
		values.resize( size );
		for ( int i = 0; i < size; i++ )
		{
			Read( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::forward_list<T>& values )
	{
		int size{};
		Read( file, size );
		std::forward_list<T> tempList{};
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			tempList.push_front( temp );
		}
		for ( const T& value : tempList )
		{
			values.push_front( value );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::list<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Read( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::stack<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			Read( file, values[i] );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::queue<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			values.push( temp );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::priority_queue<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			values.emplace( temp );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::set<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			values.emplace( temp );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::multiset<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			values.emplace( temp );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::unordered_set<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			values.emplace( temp );
		}
		return file;
	}

	template <typename T>
	std::istream& Read( std::istream& file, std::unordered_multiset<T>& values )
	{
		int size{};
		Read( file, size );
		for ( int i = 0; i < size; i++ )
		{
			T temp{};
			Write( file, temp );
			values.emplace( temp );
		}
		return file;
	}

	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::map<K, V>& values )
	{
		int size{};
		Read( file, size );
		for ( int i{}; i < size; ++i )
		{
			std::pair<K, V> temp;
			Write( file, temp );
			values.insert( temp );
		}
		return file;
	}

	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::multimap<K, V>& values )
	{
		int size{};
		Read( file, size );
		for ( int i{}; i < size; ++i )
		{
			std::pair<K, V> temp;
			Write( file, temp );
			values.insert( temp );
		}
		return file;
	}

	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::unordered_map<K, V>& values )
	{
		int size{};
		Read( file, size );
		for ( int i{}; i < size; ++i )
		{
			std::pair<K, V> temp;
			Write( file, temp );
			values.insert( temp );
		}
		return file;
	}

	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::unordered_multimap<K, V>& values )
	{
		int size{};
		Read( file, size );
		for ( int i{}; i < size; ++i )
		{
			std::pair<K, V> temp;
			Write( file, temp );
			values.insert( temp );
		}
		return file;
	}

	template <typename K, typename V>
	std::istream& Read( std::istream& file, std::pair<K, V>& values )
	{
		Read( file, values.first );
		Read( file, values.second );
		return file;
	}

}


inline std::ostream& BinaryReadWrite::Write( std::ostream& file, const std::string& value )
{
	const char* pText = value.c_str();
	const int size = static_cast< int >( value.size() );

	Write( file, size );
	return file.write( pText, size );
}

inline std::ostream& BinaryReadWrite::Write( std::ostream& file, const BalVulkan::SShaderResource& value )
{
	if ( value.stages == VK_SHADER_STAGE_VERTEX_BIT )
		Write( file, ( uint8_t ) 0 );
	else if ( value.stages == VK_SHADER_STAGE_GEOMETRY_BIT )
		Write( file, ( uint8_t ) 1 );
	else 	if ( value.stages == VK_SHADER_STAGE_FRAGMENT_BIT )
		Write( file, ( uint8_t ) 2 );

	Write( file, ( uint8_t ) value.type );
	Write( file, ( uint8_t ) value.mode );
	Write( file, value.set );
	Write( file, value.binding );
	Write( file, value.location );
	Write( file, value.inputAttachmentIndex );
	Write( file, value.vecSize );
	Write( file, value.columns );
	Write( file, value.arraySize );
	Write( file, value.offset );
	Write( file, value.size );
	Write( file, value.constantId );
	Write( file, value.qualifiers );
	Write( file, value.name );
	return file;
}

inline std::istream& BinaryReadWrite::Read( std::istream& file, std::string& value )
{
	value.clear();
	int size{};
	Read( file, size );
	if ( size == 0 )
		return file;
	const auto pBuffer{ new char[size] };
	file.read( pBuffer, size );
	value.append( pBuffer, size );
	delete[] pBuffer;
	return file;
}

inline std::istream& BinaryReadWrite::Read( std::istream& file, BalVulkan::SShaderResource& value )
{
	uint8_t shaderStage;
	uint8_t resourceType;
	uint8_t resourceMode;
	Read( file, shaderStage );
	Read( file, resourceType );
	Read( file, resourceMode );
	Read( file, value.set );
	Read( file, value.binding );
	Read( file, value.location );
	Read( file, value.inputAttachmentIndex );
	Read( file, value.vecSize );
	Read( file, value.columns );
	Read( file, value.arraySize );
	Read( file, value.offset );
	Read( file, value.size );
	Read( file, value.constantId );
	Read( file, value.qualifiers );
	Read( file, value.name );
	if ( ( BalVulkan::EShaderType ) shaderStage == BalVulkan::EShaderType::Vertex )
		value.stages = VK_SHADER_STAGE_VERTEX_BIT;
	else if ( ( BalVulkan::EShaderType ) shaderStage == BalVulkan::EShaderType::Geometry )
		value.stages = VK_SHADER_STAGE_GEOMETRY_BIT;
	else if ( ( BalVulkan::EShaderType ) shaderStage == BalVulkan::EShaderType::Fragment )
		value.stages = VK_SHADER_STAGE_FRAGMENT_BIT;

	value.type = ( BalVulkan::EShaderResourceType ) resourceType;
	value.mode = ( BalVulkan::EShaderResourceMode ) resourceMode;
	return file;
}

inline std::istream& BinaryReadWrite::IsAtStart( std::istream& file, bool& isAtStart )
{
	isAtStart = ( uint32_t ) file.tellg() == ( uint32_t ) file.beg;
	return file;
}

inline std::istream& BinaryReadWrite::IsAtEnd( std::istream& file, bool& isAtEnd )
{
	isAtEnd = file.eof();
	return file;
}

inline std::istream& BinaryReadWrite::MoveCursor( std::istream& file, int value )
{
	file.seekg( value, std::ios::cur );
	return file;
}

inline std::istream& BinaryReadWrite::MoveCursorTo( std::istream& file, int value )
{
	file.seekg( value );
	return file;
}

inline std::istream& BinaryReadWrite::MoveCursorToStart( std::istream& file )
{
	file.seekg( 0, std::ios::beg );
	return file;
}

inline std::istream& BinaryReadWrite::MoveCursorToEnd( std::istream& file )
{
	file.seekg( 0, std::ios::end );
	return file;
}

inline std::istream& BinaryReadWrite::GetCursorPosition( std::istream& file, uint64_t& size )
{
	size = file.tellg();
	return file;
}

inline std::istream& BinaryReadWrite::GetData( std::istream& file, char* pData, const int64_t size )
{
	file.read( pData, size );
	return file;
}
