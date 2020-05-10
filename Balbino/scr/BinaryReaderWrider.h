#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iterator>
#include "Struct.h"

/*
a lot of the code is inspired bey
http://forums.codeguru.com/showthread.php?269648-C-Structure-How-do-I-write-a-structure-to-a-file
*/
namespace Balbino
{
	namespace BinaryReadWrite
	{
		//write
		template<typename T>
		bool WriteToFile( const std::string& path, const T& value );

		template<typename T>
		std::ostream& Write( std::ostream& file, const T& value );
		template<typename T>
		std::ostream& Write( std::ostream& file, const T* pValue, size_t size );
		template<typename T, size_t SIZE>
		std::ostream& Write( std::ostream& file, const std::array<T, SIZE>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::vector<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::deque<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::forward_list<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::list<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::stack<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::queue<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::priority_queue<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::set<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::multiset<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::unordered_set<T>& values );
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::unordered_multiset<T>& values );
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::map<K, V>& values );
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::multimap<K, V>& values );
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::unordered_map<K, V>& values );
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::unordered_multimap<K, V>& values );
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::pair<K, V>& values );

		std::ostream& Write( std::ostream& file, const std::string& value );
		std::ostream& Write( std::ostream& file, const Balbino::Color& value );
		std::ostream& Write( std::ostream& file, const Balbino::Vector2& value );
		std::ostream& Write( std::ostream& file, const Balbino::Vector3& value );
		std::ostream& Write( std::ostream& file, const Balbino::Vector4& value );

		//read
		template<typename T>
		bool ReadToFile( std::string& path, T& value );

		template<typename T>
		std::istream& Read( std::istream& file, T& value );
		template<typename T>
		std::istream& Read( std::istream& file, const T* pValue, size_t size );
		template<typename T, size_t SIZE>
		std::istream& Read( std::istream& file, std::array<T, SIZE>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::vector<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::deque<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::forward_list<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::list<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::stack<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::queue<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::priority_queue<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::set<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::multiset<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::unordered_set<T>& values );
		template<typename T>
		std::istream& Read( std::istream& file, std::unordered_multiset<T>& values );
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::map<K, V>& values );
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::multimap<K, V>& values );
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::unordered_map<K, V>& values );
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::unordered_multimap<K, V>& values );
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::pair<K, V>& values );

		std::istream& Read( std::istream& file, std::string& value );
		std::istream& Read( std::istream& file, Balbino::Color& value );
		std::istream& Read( std::istream& file, Balbino::Vector2& value );
		std::istream& Read( std::istream& file, Balbino::Vector3& value );
		std::istream& Read( std::istream& file, Balbino::Vector4& value );

		/////////////////////////////////////////////////////////////////////////////////////////////
		//write
		template<typename T>
		bool WriteToFile( const std::string& path, const T& value )
		{
			/*if ( !std::is_pod<T>::value )
			{
				return false;
			}*/

			std::ofstream output;
			output.open( path, std::ios::out | std::ios::binary );
			if( !output.is_open() )
			{
				return false;
			}

			Write( output, value );
			output.close();
			return true;
		}

		template<typename T>
		std::ostream& Write( std::ostream& file, const T& value )
		{
			if( ( file.rdstate() & std::ifstream::failbit ) != 0 )
			{
				std::cerr << "Error writeing\n";
				return file;
			}
			if( !std::is_pod<T>::value )
			{
				return file;
			}
			return file.write( (const char*) &value, sizeof( T ) );
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const T* pValue, size_t size )
		{
			for( size_t i = 0; i < size; i++ )
			{
				Write( file, pValue[i] );
			}
			return file;
		}
		template<typename T, size_t SIZE>
		std::ostream& Write( std::ostream& file, const std::array<T, SIZE>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( int i = 0; i < size; i++ )
			{
				Write( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::vector<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			//if ( !std::is_pod<T>::value )
			//{
			//	for ( size_t i = 0; i < size; i++ )
			//	{
			//		if ( !Write( file, values[i] ) )
			//		{
			//			return file;
			//		}
			//	}
			//	return file;
			//}
			for( int i = 0; i < size; i++ )
			{
				Write( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::deque<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( int i = 0; i < size; i++ )
			{
				Write( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::forward_list<T>& values )
		{
			int size{ int( std::distance( values.begin(), values.end() ) ) };
			Write( file, size );
			for( const T& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::list<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( int i = 0; i < size; i++ )
			{
				Write( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::stack<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( int i = 0; i < size; i++ )
			{
				Write( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::queue<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			std::queue<T> coppy{ values };
			while( !coppy.empty() )
			{
				Write( file, coppy.front() );
				coppy.pop();
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::priority_queue<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			std::priority_queue<T> coppy{ values };
			while( !coppy.empty() )
			{
				Write( file, coppy.top() );
				coppy.pop();
			}

			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::set<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const T& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::multiset<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const T& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::unordered_set<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const T& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename T>
		std::ostream& Write( std::ostream& file, const std::unordered_multiset<T>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const T& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::map<K, V>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const std::pair<K, V>& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::multimap<K, V>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const std::pair<K, V>& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::unordered_map<K, V>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const std::pair<K, V>& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::unordered_multimap<K, V>& values )
		{
			int size{ int( values.size() ) };
			Write( file, size );
			for( const std::pair<K, V>& value : values )
			{
				Write( file, value );
			}
			return file;
		}
		template<typename K, typename V>
		std::ostream& Write( std::ostream& file, const std::pair<K, V>& values )
		{
			Write( file, values.first );
			Write( file, values.second );
			return file;
		}

		//read
		template<typename T>
		bool ReadToFile( const std::string& path, T& value )
		{
			/*if ( !std::is_pod<T>::value )
			{
				return false;
			}*/

			std::ifstream output;
			output.open( path, std::ios::in | std::ios::binary );
			if( !output.is_open() )
			{
				return false;
			}

			Read( output, value );
			output.close();
			return true;
		}

		template<typename T>
		std::istream& Read( std::istream& file, T& value )
		{
			if( ( file.rdstate() & std::ifstream::failbit ) != 0 )
			{
				std::cerr << "Error reading\n";
				return file;
			}
			if( !std::is_pod<T>::value )
			{
				return file;
			}
			return file.read( (char*) &value, sizeof( T ) );
		}
		template<typename T>
		std::istream& Read( std::istream& file, const T* pValue, size_t size )
		{
			for( size_t i = 0; i < size; i++ )
			{
				Read( file, pValue[i] );
			}
		}
		template<typename T, size_t SIZE>
		std::istream& Read( std::istream& file, std::array<T, SIZE>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				Read( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::vector<T>& values )
		{
			int size{ int( values.size() ) };
			Read( file, size );
			values.resize( size );
			//if ( !std::is_pod<T>::value )
			//{
			//	for ( size_t i = 0; i < size; i++ )
			//	{
			//		if ( !Read( file, values[i] ) )
			//		{
			//			return file;
			//		}
			//	}
			//	return file;
			//}
			for( int i = 0; i < size; i++ )
			{
				Read( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::deque<T>& values )
		{
			int size{};
			Read( file, size );
			values.resize( size );
			for( int i = 0; i < size; i++ )
			{
				Read( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::forward_list<T>& values )
		{
			int size{};
			Read( file, size );
			std::forward_list<T> tempList{};
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				tempList.push_front( temp );
			}
			for( const T& value : tempList )
			{
				values.push_front( value );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::list<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				Read( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::stack<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				Read( file, values[i] );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::queue<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				values.push( temp );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::priority_queue<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				values.emplace( temp );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::set<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				values.emplace( temp );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::multiset<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				values.emplace( temp );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::unordered_set<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				values.emplace( temp );
			}
			return file;
		}
		template<typename T>
		std::istream& Read( std::istream& file, std::unordered_multiset<T>& values )
		{
			int size{};
			Read( file, size );
			for( int i = 0; i < size; i++ )
			{
				T temp;
				Read( file, temp );
				values.emplace( temp );
			}
			return file;
		}
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::map<K, V>& values )
		{
			int size{};
			Read( file, size );
			for( int i{}; i < size; ++i )
			{
				std::pair<K, V> temp;
				Read( file, temp );
				values.insert( temp );
			}
			return file;
		}
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::multimap<K, V>& values )
		{
			int size{};
			Read( file, size );
			for( int i{}; i < size; ++i )
			{
				std::pair<K, V> temp;
				Read( file, temp );
				values.insert( temp );
			}
			return file;
		}
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::unordered_map<K, V>& values )
		{
			int size{};
			Read( file, size );
			for( int i{}; i < size; ++i )
			{
				std::pair<K, V> temp;
				Read( file, temp );
				values.insert( temp );
			}
			return file;
		}
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::unordered_multimap<K, V>& values )
		{
			int size{};
			Read( file, size );
			for( int i{}; i < size; ++i )
			{
				std::pair<K, V> temp;
				Read( file, temp );
				values.insert( temp );
			}
			return file;
		}
		template<typename K, typename V>
		std::istream& Read( std::istream& file, std::pair<K, V>& values )
		{
			Read( file, values.first );
			Read( file, values.second );
			return file;
		}
	}
}