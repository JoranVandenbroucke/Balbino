#pragma once
#include <random>
#include <xhash>

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine( s_randomDevice() );
static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

class CUuid final
{
public:
	CUuid()
		: m_uuid{ s_uniformDistribution( s_engine ) }
	{
	}
	CUuid( uint64_t uuid )
		: m_uuid{ uuid }
	{
	}
	CUuid( const CUuid& ) = default;
	CUuid( CUuid&& ) = default;
	CUuid& operator=( CUuid&& ) = default;
	CUuid& operator=( const CUuid& ) = default;
	~CUuid() = default;

	explicit operator uint64_t() const
	{
		return m_uuid;
	}

private:
	uint64_t m_uuid;
};


namespace std
{
	template<>
	struct hash<CUuid>
	{
		std::size_t operator()( const CUuid& uuid ) const noexcept
		{
			return hash<uint64_t>()( static_cast< uint64_t >( uuid ) );
		}
	};
}