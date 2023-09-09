#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <forward_list>
#include <fstream>

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

#include "../Utilities/UUID.h"
#include "FawnVision.h"
#include "MeshMetadata.h"

enum class file_type : uint8_t
{
    unknown,
    folder,
    audio,
    code,
    font,
    image,
    material,
    model,
    preset,
    scene,
    shader,
    max
};

constexpr const char* ToString( file_type type )
{
    switch ( type )
    {
        case file_type::unknown: return "Unknown";
        case file_type::folder: return "Folder";
        case file_type::scene: return "Scene";
        case file_type::code: return "Code";
        case file_type::image: return "image";
        case file_type::audio: return "Audio";
        case file_type::font: return "Font";
        case file_type::model: return "Model";
        case file_type::preset: return "Preset";
        case file_type::shader: return "Shader";
        case file_type::material: return "Material";
        case file_type::max: break;
    }
    return "";
}

struct SFile final {
    SFile() = default;
    SFile( std::string p, std::string n )
        : fileName { std::move( n ) }
        , path { std::move( p ) }
    {
    }
    bool isFolder { true };
    file_type type { file_type::folder };
    CUuid uuid {};
    std::string fileName {};
    std::string path {};
    std::filesystem::file_time_type lastWrittenTime {};
    int depth {};

    bool operator==( const CUuid& id ) const
    {
        return id == uuid;
    }

    bool operator==( const std::string_view otherName ) const
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

namespace Serialization
{
#pragma region Write
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const T& value )
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
        return file.write( reinterpret_cast<const char*>( &value ), sizeof( T ) );
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const T* pValue, uint64_t size )
    {
        return file.write( reinterpret_cast<const char*>( pValue ), size );
    }
    template<typename T, uint64_t SIZE>
    inline std::ostream& Write( std::ostream& file, const std::array<T, SIZE>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::vector<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::deque<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::forward_list<T>& values )
    {
        Write( file, static_cast<int>( std::distance( values.begin(), values.end() ) ) );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::list<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::stack<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Write( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::queue<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        std::queue<T> copy { values };
        while ( !copy.empty() )
        {
            Write( file, copy.front() );
            copy.pop();
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::priority_queue<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        std::priority_queue<T> copy { values };
        while ( !copy.empty() )
        {
            Write( file, copy.top() );
            copy.pop();
        }

        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::set<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::multiset<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::unordered_set<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename T>
    inline std::ostream& Write( std::ostream& file, const std::unordered_multiset<T>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const T& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::ostream& Write( std::ostream& file, const std::map<K, V>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::ostream& Write( std::ostream& file, const std::multimap<K, V>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::ostream& Write( std::ostream& file, const std::unordered_map<K, V>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::ostream& Write( std::ostream& file, const std::unordered_multimap<K, V>& values )
    {
        const int size { static_cast<int>( values.size() ) };
        Write( file, size );
        for ( const std::pair<K, V>& value : values )
        {
            Write( file, value );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::ostream& Write( std::ostream& file, const std::pair<K, V>& values )
    {
        Write( file, values.first );
        Write( file, values.second );
        return file;
    }

    inline std::ostream& Write( std::ostream& file, const SMeshMetadata& value )
    {
        Serialization::Write( file, value.boundingBox.min.x );
        Serialization::Write( file, value.boundingBox.min.y );
        Serialization::Write( file, value.boundingBox.min.z );
        Serialization::Write( file, value.boundingBox.max.x );
        Serialization::Write( file, value.boundingBox.max.y );
        Serialization::Write( file, value.boundingBox.max.z );
        Serialization::Write( file, value.boundingSphere.center.x );
        Serialization::Write( file, value.boundingSphere.center.y );
        Serialization::Write( file, value.boundingSphere.center.z );
        Serialization::Write( file, value.boundingSphere.radius );
        Serialization::Write( file, value.firstIndex );
        Serialization::Write( file, value.indexCount );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SShaderResource& value )
    {
        Write( file, value.name );
        Write( file, value.resourceId );
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
        Write( file, value.stages );
        Write( file, value.type );
        Write( file, value.mode );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SImageCreateInfo& value )
    {
        Write( file, value.arrayLayers );
        Write( file, value.depth );
        Write( file, value.format );
        Write( file, value.height );
        Write( file, value.initialLayout );
        Write( file, value.mipLevels );
        Write( file, value.samples );
        Write( file, value.usage );
        Write( file, value.width );
        Write( file, value.imageType );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SImageViewCreateInfo& value )
    {
        Write( file, value.componentMapping );
        Write( file, value.firstLayer );
        Write( file, value.firstMip );
        Write( file, value.layerCount );
        Write( file, value.mipCount );
        Write( file, value.imageViewType );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SSamplerCreateInfo& value )
    {
        Write( file, value.maxAnisotropy );
        Write( file, value.maxLod );
        Write( file, value.minLod );
        Write( file, value.mipLodBias );
        Write( file, value.compareEnable );
        Write( file, value.unNormalizedCoordinates );
        Write( file, value.addressModeU );
        Write( file, value.addressModeV );
        Write( file, value.addressModeW );
        Write( file, value.minFilter );
        Write( file, value.mipMapMode );
        Write( file, value.borderColor );
        Write( file, value.compareOperator );
        Write( file, value.magFilter );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::STextureCreateInfo& value )
    {
        Write( file, value.imageCreateInfo );
        Write( file, value.imageViewCreateInfo );
        Write( file, value.samplerCreateInfo );
        Write( file, value.imageDataSize );
        Write( file, value.pImageData, value.imageDataSize );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SShaderCreateInfo& value )
    {
        Write( file, value.type );
        Write( file, value.size );
        Write( file, value.pData );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SShaderPassCreateInfo& value )
    {
        Write( file, value.blendAttachmentSize );
        Write( file, value.types );
        Write( file, value.components );
        Write( file, value.cullModeFlag );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SMaterialCreateInfo& value )
    {
        Write( file, value.shaderId );
        Write( file, value.shaderResources );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SMeshCreateInfo& value )
    {
        Write( file, value.indices );
        Write( file, value.vertices );
        Write( file, value.metadata );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SComponentMapping& value )
    {
        Write( file, (uint8_t)value.r );
        Write( file, (uint8_t)value.g );
        Write( file, (uint8_t)value.b );
        Write( file, (uint8_t)value.a );
        return file;
    }
    inline std::ostream& Write( std::ostream& file, const FawnVision::SVertex& value )
    {
        Write( file, value.position.x );
        Write( file, value.position.y );
        Write( file, value.position.z );

        Write( file, value.color.r );
        Write( file, value.color.g );
        Write( file, value.color.b );
        Write( file, value.color.a );

        Write( file, value.uv.x );
        Write( file, value.uv.y );

        Write( file, value.normal.x );
        Write( file, value.normal.y );
        Write( file, value.normal.z );

        Write( file, value.tangent.x );
        Write( file, value.tangent.y );
        Write( file, value.tangent.z );
        Write( file, value.tangent.w );

        return file;
    }
    inline std::ostream& Write( std::ostream& file, const std::string& value )
    {
        const char* pText = value.c_str();
        const auto size   = static_cast<int>( value.size() );

        Write( file, size );
        return file.write( pText, size );
    }
#pragma endregion Write
#pragma region Read
    template<typename T>
    inline std::istream& Read( std::istream& file, T& value )
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
        return file.read( reinterpret_cast<char*>( &value ), sizeof( T ) );
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, T*& pValue, uint64_t size )
    {
        return file.read( reinterpret_cast<char*>( pValue ), size );
    }
    template<typename T, uint64_t SIZE>
    inline std::istream& Read( std::istream& file, std::array<T, SIZE>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Read( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::vector<T>& values )
    {
        int size { static_cast<int>( values.size() ) };
        Read( file, size );
        values.resize( size );
        for ( int i = 0; i < size; i++ )
        {
            Read( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::deque<T>& values )
    {
        int size {};
        Read( file, size );
        values.resize( size );
        for ( int i = 0; i < size; i++ )
        {
            Read( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::forward_list<T>& values )
    {
        int size {};
        Read( file, size );
        std::forward_list<T> tempList {};
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
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
    inline std::istream& Read( std::istream& file, std::list<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Read( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::stack<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            Read( file, values[ i ] );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::queue<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
            Write( file, temp );
            values.push( temp );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::priority_queue<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
            Write( file, temp );
            values.emplace( temp );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::set<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
            Write( file, temp );
            values.emplace( temp );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::multiset<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
            Write( file, temp );
            values.emplace( temp );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::unordered_set<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
            Write( file, temp );
            values.emplace( temp );
        }
        return file;
    }
    template<typename T>
    inline std::istream& Read( std::istream& file, std::unordered_multiset<T>& values )
    {
        int size {};
        Read( file, size );
        for ( int i = 0; i < size; i++ )
        {
            T temp {};
            Write( file, temp );
            values.emplace( temp );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::istream& Read( std::istream& file, std::map<K, V>& values )
    {
        int size {};
        Read( file, size );
        for ( int i {}; i < size; ++i )
        {
            std::pair<K, V> temp;
            Write( file, temp );
            values.insert( temp );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::istream& Read( std::istream& file, std::multimap<K, V>& values )
    {
        int size {};
        Read( file, size );
        for ( int i {}; i < size; ++i )
        {
            std::pair<K, V> temp;
            Write( file, temp );
            values.insert( temp );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::istream& Read( std::istream& file, std::unordered_map<K, V>& values )
    {
        int size {};
        Read( file, size );
        for ( int i {}; i < size; ++i )
        {
            std::pair<K, V> temp;
            Write( file, temp );
            values.insert( temp );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::istream& Read( std::istream& file, std::unordered_multimap<K, V>& values )
    {
        int size {};
        Read( file, size );
        for ( int i {}; i < size; ++i )
        {
            std::pair<K, V> temp;
            Write( file, temp );
            values.insert( temp );
        }
        return file;
    }
    template<typename K, typename V>
    inline std::istream& Read( std::istream& file, std::pair<K, V>& values )
    {
        Read( file, values.first );
        Read( file, values.second );
        return file;
    }

    inline std::istream& Read( std::istream& file, SMeshMetadata& value )
    {
        Serialization::Read( file, value.boundingBox.min.x );
        Serialization::Read( file, value.boundingBox.min.y );
        Serialization::Read( file, value.boundingBox.min.z );
        Serialization::Read( file, value.boundingBox.max.x );
        Serialization::Read( file, value.boundingBox.max.y );
        Serialization::Read( file, value.boundingBox.max.z );
        Serialization::Read( file, value.boundingSphere.center.x );
        Serialization::Read( file, value.boundingSphere.center.y );
        Serialization::Read( file, value.boundingSphere.center.z );
        Serialization::Read( file, value.boundingSphere.radius );
        Serialization::Read( file, value.firstIndex );
        Serialization::Read( file, value.indexCount );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SShaderResource& value )
    {
        Read( file, value.name );
        Read( file, value.resourceId );
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
        Read( file, value.stages );
        Read( file, value.type );
        Read( file, value.mode );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SImageCreateInfo& value )
    {
        Read( file, value.arrayLayers );
        Read( file, value.depth );
        Read( file, value.format );
        Read( file, value.height );
        Read( file, value.initialLayout );
        Read( file, value.mipLevels );
        Read( file, value.samples );
        Read( file, value.usage );
        Read( file, value.width );
        Read( file, value.imageType );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SImageViewCreateInfo& value )
    {
        Read( file, value.componentMapping );
        Read( file, value.firstLayer );
        Read( file, value.firstMip );
        Read( file, value.layerCount );
        Read( file, value.mipCount );
        Read( file, value.imageViewType );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SSamplerCreateInfo& value )
    {
        Read( file, value.maxAnisotropy );
        Read( file, value.maxLod );
        Read( file, value.minLod );
        Read( file, value.mipLodBias );
        Read( file, value.compareEnable );
        Read( file, value.unNormalizedCoordinates );
        Read( file, value.addressModeU );
        Read( file, value.addressModeV );
        Read( file, value.addressModeW );
        Read( file, value.minFilter );
        Read( file, value.mipMapMode );
        Read( file, value.borderColor );
        Read( file, value.compareOperator );
        Read( file, value.magFilter );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::STextureCreateInfo& value )
    {
        Read( file, value.imageCreateInfo );
        Read( file, value.imageViewCreateInfo );
        Read( file, value.samplerCreateInfo );
        Read( file, value.imageDataSize );
        value.pImageData = malloc( value.imageDataSize );
        Read( file, value.pImageData, value.imageDataSize );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SShaderCreateInfo& value )
    {
        Read( file, value.type );
        Read( file, value.size );
        value.pData = (uint32_t*)malloc( value.size );
        Read( file, value.pData );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SShaderPassCreateInfo& value )
    {
        Read( file, value.blendAttachmentSize );
        Read( file, value.types );
        Read( file, value.components );
        Read( file, value.cullModeFlag );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SMaterialCreateInfo& value )
    {
        Read( file, value.shaderId );
        Read( file, value.shaderResources );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SMeshCreateInfo& value )
    {
        Read( file, value.indices );
        Read( file, value.vertices );
        Read( file, value.metadata );
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SComponentMapping& value )
    {
        uint8_t v {};
        Read( file, v );
        value.r = (FawnVision::component_swizzle)v;
        Read( file, v );
        value.g = (FawnVision::component_swizzle)v;
        Read( file, v );
        value.b = (FawnVision::component_swizzle)v;
        Read( file, v );
        value.a = (FawnVision::component_swizzle)v;
        return file;
    }
    inline std::istream& Read( std::istream& file, FawnVision::SVertex& value )
    {
        Read( file, value.position.x );
        Read( file, value.position.y );
        Read( file, value.position.z );

        Read( file, value.color.r );
        Read( file, value.color.g );
        Read( file, value.color.b );
        Read( file, value.color.a );

        Read( file, value.uv.x );
        Read( file, value.uv.y );

        Read( file, value.normal.x );
        Read( file, value.normal.y );
        Read( file, value.normal.z );

        Read( file, value.tangent.x );
        Read( file, value.tangent.y );
        Read( file, value.tangent.z );
        Read( file, value.tangent.w );
        return file;
    }
    inline std::istream& Read( std::istream& file, std::string& value )
    {
        value.clear();
        int size {};
        Read( file, size );
        if ( size == 0 )
        {
            return file;
        }
        const auto pBuffer { new char[ size ] };
        file.read( pBuffer, size );
        value.append( pBuffer, size );
        delete[] pBuffer;
        return file;
    }
#pragma endregion Read
#pragma region Others
    inline std::istream& GetCursorPosition( std::istream& file, uint64_t& size )
    {
        size = file.tellg();
        return file;
    }
    inline std::istream& MoveCursorTo( std::istream& file, int value )
    {
        file.seekg( value );
        return file;
    }
    inline std::istream& MoveCursorToStart( std::istream& file )
    {
        file.seekg( 0, std::ios::beg );
        return file;
    }
    inline std::istream& MoveCursorToEnd( std::istream& file )
    {
        file.seekg( 0, std::ios::end );
        return file;
    }
    inline std::istream& IsAtStart( std::istream& file, bool& isAtStart )
    {
        isAtStart = (uint32_t)file.tellg() <= (uint32_t)std::istream::beg;
        return file;
    }
    inline std::istream& IsAtEnd( std::istream& file, bool& isAtEnd )
    {
        auto current = (int)file.tellg();
        MoveCursorToEnd( file );
        auto end = (int)file.tellg() - 1;
        MoveCursorTo( file, current );
        isAtEnd = current >= end;
        return file;
    }
    inline std::istream& GetData( std::istream& file, char* pData, const int64_t size )
    {
        file.read( pData, size );
        return file;
    }
#pragma endregion Others
}// namespace Serialization

inline std::string SanitizePath( std::string path ) noexcept
{
    std::ranges::replace( path, '\\', '/' );
    if ( std::filesystem::is_directory( path ) )
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
