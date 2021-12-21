#include "UUID.h"


CUuid::CUuid( uint64_t uuid )
	: m_uuid{ uuid }
{
}

CUuid::operator unsigned long long() const
{
	return m_uuid;
}
