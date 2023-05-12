#pragma once

#include <algorithm>
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
#include "MeshMetadata.h"

enum file_type : uint8_t
{
    file_type_unknown,
    file_type_folder,
    file_type_scene,
    file_type_image,
    file_type_audio,
    file_type_font,
    file_type_model,
    file_type_shader,
    file_type_material,
    file_typec_code,
    file_type_preset,
    file_type_max
};

inline const char* ToString( file_type type )
{
    switch ( type )
    {
        case file_type::file_type_unknown:
            return "Unknown";
        case file_type::file_type_folder:
            return "Folder";
        case file_type::file_type_scene:
            return "Scene";
        case file_type::file_typec_code:
            return "Code";
        case file_type::file_type_image:
            return "Image";
        case file_type::file_type_audio:
            return "Audio";
        case file_type::file_type_font:
            return "Font";
        case file_type::file_type_model:
            return "Model";
        case file_type::file_type_preset:
            return "Preset";
        case file_type::file_type_shader:
            return "Shader";
        case file_type::file_type_material:
            return "Material";
        case file_type::file_type_max:
            break;
    }
    return "";
}

struct SFile
{
    bool      isFolder;
    file_type type;
    uint64_t  uuid;
    std::string                     fileName;
    std::string                     path;
    std::filesystem::file_time_type lastWrittenTime;
    int                             depth;
    
    bool operator==( const uint64_t& id ) const
    {
        return id == uuid;
    }
    
    bool operator==( const CUuid& id ) const
    {
        return (uint64_t) id == uuid;
    }
    
    bool operator==( const std::string& otherName ) const
    {
        return otherName == fileName;
    }
    
    bool operator==( const std::filesystem::path& otherPath ) const
    {
        return otherPath == path;
    }
    
    bool operator==( const SFile& other ) const
    {
        return this->uuid == other.uuid;
    }
};

namespace BinaryReadWrite
{
    //write
    template<typename T>
    std::ostream& Write( std::ostream& file, const T& value );
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const T* pValue, uint64_t size );
    
    template<typename T, uint64_t SIZE>
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
    
    std::ostream& Write( std::ostream& file, const FawnVision::SShaderResource& value );
    
    std::ostream& Write( std::ostream& file, const SMeshMetadata& value );
    
    //read
    template<typename T>
    std::istream& Read( std::istream& file, T& value );
    
    template<typename T>
    std::istream& Read( std::istream& file, T*& pValue, uint64_t size );
    
    template<typename T, uint64_t SIZE>
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
    
    std::istream& Read( std::istream& file, FawnVision::SShaderResource& value );
    
    std::istream& Read( std::istream& file, SMeshMetadata& value );
    
    /////////////////////////////////////////////////////////////////////////////////////////////
    std::istream& IsAtStart( std::istream& file, bool& isAtStart );
    
    std::istream& IsAtEnd( std::istream& file, bool& isAtEnd );
    
    std::istream& MoveCursor( std::istream& file, int64_t value );
    
    std::istream& MoveCursorTo( std::istream& file, int value );
    
    std::istream& MoveCursorToStart( std::istream& file );
    
    std::istream& MoveCursorToEnd( std::istream& file );
    
    std::istream& GetCursorPosition( std::istream& file, uint64_t& size );
    
    std::istream& GetData( std::istream& file, char* pData, int64_t size );
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const T& value )
    {
        if (( file.rdstate() & std::ifstream::failbit ) != 0 )
        {
            std::cerr << "Error writing\n";
            return file;
        }
        if ( !std::is_standard_layout_v<T> )
        {
            return file;
        }
        return file.write( reinterpret_cast< const char* >( &value ), sizeof( T ));
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const T* pValue, uint64_t size )
    {
        return file.write( reinterpret_cast< const char* >( pValue ), size );
    }
    
    template<typename T, uint64_t SIZE>
    std::ostream& Write( std::ostream& file, const std::array<T, SIZE>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[i] );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::vector<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[i] );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::deque<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[i] );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::forward_list<T>& values )
    {
        Write( file, static_cast< int >( std::distance( values.begin(), values.end())));
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::list<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[i] );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::stack<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[i] );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::queue<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        std::queue<T> copy{ values };
        while ( !copy.empty())
        {
            Write( file, copy.front());
            copy.pop();
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::priority_queue<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        std::priority_queue<T> copy{ values };
        while ( !copy.empty())
        {
            Write( file, copy.top());
            copy.pop();
        }
        
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::set<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::multiset<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::unordered_set<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename T>
    std::ostream& Write( std::ostream& file, const std::unordered_multiset<T>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename K, typename V>
    std::ostream& Write( std::ostream& file, const std::map<K, V>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename K, typename V>
    std::ostream& Write( std::ostream& file, const std::multimap<K, V>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename K, typename V>
    std::ostream& Write( std::ostream& file, const std::unordered_map<K, V>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    
    template<typename K, typename V>
    std::ostream& Write( std::ostream& file, const std::unordered_multimap<K, V>& values )
    {
        const int size{ static_cast< int >( values.size()) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
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
    std::istream& Read( std::istream& file, T& value )
    {
        if (( file.rdstate() & std::ifstream::failbit ) != 0 )
        {
            std::cerr << "Error reading\n";
            return file;
        }
        if ( !std::is_standard_layout_v<T> )
        {
            return file;
        }
        return file.read( reinterpret_cast< char* >( &value ), sizeof( T ));
    }
    
    template<typename T>
    std::istream& Read( std::istream& file, T*& pValue, uint64_t size )
    {
        return file.read( reinterpret_cast< char* >( pValue ), size );
    }
    
    template<typename T, uint64_t SIZE>
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
    
    template<typename T>
    std::istream& Read( std::istream& file, std::vector<T>& values )
    {
        int size{ static_cast< int >( values.size()) };
        Read( file, size );
        values.resize( size );
        for ( int i = 0; i < size; i++ )
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
        for ( int i = 0; i < size; i++ )
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
        for ( int            i = 0; i < size; i++ )
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename T>
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
    
    template<typename K, typename V>
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
    
    template<typename K, typename V>
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
    
    template<typename K, typename V>
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
    
    template<typename K, typename V>
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
    
    template<typename K, typename V>
    std::istream& Read( std::istream& file, std::pair<K, V>& values )
    {
        Read( file, values.first );
        Read( file, values.second );
        return file;
    }
    
}

inline std::string SanitizePath( std::string path )
{
    std::replace(path.begin(), path.end(), '\\', '/');
    if ( std::filesystem::is_directory( path ))
    {
        if ( path.back() != '/' )
        {
            path += '/';
        }
    }
    else
    {
        path = path.substr( 0, path.find_last_of( '/' ) + 1 );
    }
    return path;
}

inline std::ostream& BinaryReadWrite::Write( std::ostream& file, const std::string& value )
{
    const char* pText = value.c_str();
    const int size = static_cast< int >( value.size());
    
    Write( file, size );
    return file.write( pText, size );
}

inline std::ostream& BinaryReadWrite::Write( std::ostream& file, const FawnVision::SShaderResource& value )
{
    if ( value.stages == VK_SHADER_STAGE_VERTEX_BIT )
    {
        Write( file, (uint8_t) 0u );
    }
    else if ( value.stages == VK_SHADER_STAGE_GEOMETRY_BIT )
    {
        Write( file, (uint8_t) 1 );
    }
    else if ( value.stages == VK_SHADER_STAGE_FRAGMENT_BIT )
    {
        Write( file, (uint8_t) 2 );
    }
    
    Write( file, (uint8_t) value.type );
    Write( file, (uint8_t) value.mode );
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
    Write( file, value.resourceID );
    Write( file, value.name );
    return file;
}

inline std::istream& BinaryReadWrite::Read( std::istream& file, std::string& value )
{
    value.clear();
    int size{};
    Read( file, size );
    if ( size == 0 )
    {
        return file;
    }
    const auto pBuffer{ new char[size] };
    file.read( pBuffer, size );
    value.append( pBuffer, size );
    delete[] pBuffer;
    return file;
}

inline std::istream& BinaryReadWrite::Read( std::istream& file, FawnVision::SShaderResource& value )
{
    uint8_t resourceType;
    uint8_t resourceMode;
    uint16_t shaderStage;
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
    Read( file, value.resourceID );
    Read( file, value.name );
    // todo: update
    if ((FawnVision::shader_stage) shaderStage == FawnVision::shader_stage_vertex )
    {
        value.stages = VK_SHADER_STAGE_VERTEX_BIT;
    }
    else if ((FawnVision::shader_stage) shaderStage == FawnVision::shader_stage_geometry )
    {
        value.stages = VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    else if ((FawnVision::shader_stage) shaderStage == FawnVision::shader_stage_fragment )
    {
        value.stages = VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    
    value.type = (FawnVision::EShaderResourceType::Enum) resourceType;
    value.mode = (FawnVision::EShaderResourceMode::Enum) resourceMode;
    return file;
}

inline std::istream& BinaryReadWrite::IsAtStart( std::istream& file, bool& isAtStart )
{
    isAtStart = (uint32_t) file.tellg() <= (uint32_t) std::istream::beg;
    return file;
}

inline std::istream& BinaryReadWrite::IsAtEnd( std::istream& file, bool& isAtEnd )
{
    auto current = (int) file.tellg();
    MoveCursorToEnd( file );
    auto end = (int) file.tellg() - 1;
    MoveCursorTo( file, current );
    isAtEnd = current >= end;
//    isAtEnd = file.eof();
    return file;
}

inline std::istream& BinaryReadWrite::MoveCursor( std::istream& file, int64_t value )
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
inline std::ostream& BinaryReadWrite::Write( std::ostream& file, const SMeshMetadata& value )
{
    BinaryReadWrite::Write( file, value.boundingBox.min.x );
    BinaryReadWrite::Write( file, value.boundingBox.min.y );
    BinaryReadWrite::Write( file, value.boundingBox.min.z );
    BinaryReadWrite::Write( file, value.boundingBox.max.x );
    BinaryReadWrite::Write( file, value.boundingBox.max.y );
    BinaryReadWrite::Write( file, value.boundingBox.max.z );
    BinaryReadWrite::Write( file, value.boundingSphere.center.x );
    BinaryReadWrite::Write( file, value.boundingSphere.center.y );
    BinaryReadWrite::Write( file, value.boundingSphere.center.z );
    BinaryReadWrite::Write( file, value.boundingSphere.radius );
    BinaryReadWrite::Write( file, value.firstIndex );
    BinaryReadWrite::Write( file, value.indexCount );
    return file;
}
inline std::istream& BinaryReadWrite::Read( std::istream& file, SMeshMetadata& value )
{
    BinaryReadWrite::Read( file, value.boundingBox.min.x );
    BinaryReadWrite::Read( file, value.boundingBox.min.y );
    BinaryReadWrite::Read( file, value.boundingBox.min.z );
    BinaryReadWrite::Read( file, value.boundingBox.max.x );
    BinaryReadWrite::Read( file, value.boundingBox.max.y );
    BinaryReadWrite::Read( file, value.boundingBox.max.z );
    BinaryReadWrite::Read( file, value.boundingSphere.center.x );
    BinaryReadWrite::Read( file, value.boundingSphere.center.y );
    BinaryReadWrite::Read( file, value.boundingSphere.center.z );
    BinaryReadWrite::Read( file, value.boundingSphere.radius );
    BinaryReadWrite::Read( file, value.firstIndex );
    BinaryReadWrite::Read( file, value.indexCount );
    return file;
}
