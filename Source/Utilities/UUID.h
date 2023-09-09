#pragma once
#include <iomanip>
#include <random>
#include <sstream>

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine( s_randomDevice() );
static std::uniform_int_distribution<uint32_t> s_uniformDistribution;

class CUuid final
{
public:
    CUuid()
        : m_uuid {}
    {
        GenerateUUID();
    }

    explicit CUuid( const char* uuid )
        : m_uuid {}
    {
        std::memcpy( m_uuid, uuid, 16 );
    }
    explicit CUuid( const std::string& uuid )
        : m_uuid {}
    {
        if ( uuid.size() >= 16 )
            std::memcpy( m_uuid, uuid.data(), 16 );
    }

    constexpr CUuid( const CUuid& )  = default;
    constexpr CUuid( CUuid&& )       = default;
    CUuid& operator=( CUuid&& )      = default;
    CUuid& operator=( const CUuid& ) = default;
    constexpr ~CUuid()               = default;

    inline CUuid& operator=( const std::string& uuid )
    {
        return *this = CUuid { uuid };
    }
    constexpr explicit operator const char8_t*() const
    {
        return m_uuid;
    }
    [[nodiscard]] std::string ToString() const
    {
        char hexChars[ 37 ];
        int pos = 0;

        for ( int i = 0; i < 16; ++i )
        {
            pos += sprintf_s( hexChars + pos, 37 - pos, "%02x", static_cast<uint32_t>( m_uuid[ i ] ) );
            if ( i == 3 || i == 5 || i == 7 || i == 9 )
            {
                hexChars[ pos++ ] = '-';
            }
        }

        hexChars[ 36 ] = '\0';
        return hexChars;
    }
    inline bool operator==( const CUuid& other ) const
    {
        return memcmp( m_uuid, other.m_uuid, sizeof( m_uuid ) ) == 0;
    }
    inline bool operator!=( const CUuid& other ) const
    {
        return !( *this == other );
    }
    constexpr char8_t operator[]( int idx ) const
    {
        return m_uuid[ idx ];
    }

    constexpr std::string Data() const
    {
        std::u8string_view v( m_uuid );
        return { v.cbegin(), v.cend() };
    }

private:
    char8_t m_uuid[ 16 ];
    void GenerateUUID()
    {
        for ( int i {}; i < 16; i += 4 )
        {
            uint32_t chunk = s_uniformDistribution( s_engine );
            memcpy( m_uuid + i, &chunk, sizeof( uint32_t ) );
        }

        // Set UUID version (4) and variant bits
        m_uuid[ 6 ] = ( m_uuid[ 6 ] & 0x0F ) | 0x40;// Version 4
        m_uuid[ 8 ] = ( m_uuid[ 8 ] & 0x3F ) | 0x80;// Variant (RFC 4122)

        // Note: This is a simple example and doesn't guarantee cryptographic-level randomness.
    }
};

namespace std
{
    template<>
    struct hash<CUuid> {
        constexpr std::size_t operator()( const CUuid& uuid ) const noexcept
        {
            std::size_t hash = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 0 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 1 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 2 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 3 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 4 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 5 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 6 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 7 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 8 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 9 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 10 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 11 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 12 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 13 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 14 ] );
            hash             = ( hash * 31 ) ^ static_cast<std::size_t>( uuid[ 15 ] );
            return hash;
        }
    };
}// namespace std
