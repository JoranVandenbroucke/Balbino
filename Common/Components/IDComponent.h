#pragma once
#include "../UUID.h"
#include "../IEntity.h"

class CIDComponent final
{
public:
	CIDComponent() = default;

	explicit CIDComponent( IEntity* pEntity, const CUuid& id )
		: m_id{ id }
		, m_pEntity{ pEntity }
	{
	}
	~CIDComponent() = default;
	CIDComponent( const CIDComponent& ) = default;
	CIDComponent( CIDComponent&& ) = default;
	CIDComponent& operator=( const CIDComponent& ) = default;
	CIDComponent& operator=( CIDComponent&& ) = default;

	[[nodiscard]] const CUuid& GetUUID()const
	{
		return m_id;
	}

	IEntity* GetEntity()const
	{
		return m_pEntity;
	}
private:
	CUuid m_id;
	IEntity* m_pEntity;
};

