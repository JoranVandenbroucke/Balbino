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
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class EFileTypes : uint8_t
{
	Unknown,
	Folder,
	Scene,
	Code,
	Image,
	Audio,
	Font,
	Model,
	Preset
};

const struct SFile
{
	bool isFolder;
	EFileTypes type;
	std::string alias;
	std::filesystem::path path;
	uint64_t size;
	void* pData;
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

	//read
	template <typename T>
	std::istream& Read( std::istream& file, T& value );
	template <typename T>
	std::istream& Read( std::istream& file, T* pValue, uint64_t size );
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
	/////////////////////////////////////////////////////////////////////////////////////////////

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
		return file.write( (const char*) pValue, size );
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
	std::istream& Read( std::istream& file, T* pValue, uint64_t size )
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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
			Read( file, temp );
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