#include "pch.h"
#include "IDComponent.h"
#include "IEntity.h"

Balbino::CIDComponent::CIDComponent( IEntity* pEntity, const CUuid& id )
	: m_id{ id }
	, m_pEntity{ pEntity }
{
}

const CUuid& Balbino::CIDComponent::GetUUID() const
{
	return m_id;
}

IEntity* Balbino::CIDComponent::GetEntity() const
{
	return m_pEntity;
}
