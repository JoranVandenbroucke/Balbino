#pragma once
#include "UUID.h"

struct IEntity;

namespace Balbino
{
	class CIDComponent final
	{
	public:
		CIDComponent() = default;

		explicit CIDComponent( IEntity* pEntity, const CUuid& id );
		~CIDComponent() = default;
		CIDComponent( const CIDComponent& ) = default;
		CIDComponent( CIDComponent&& ) = default;
		CIDComponent& operator=( const CIDComponent& ) = default;
		CIDComponent& operator=( CIDComponent&& ) = default;

		[[nodiscard]] const CUuid& GetUUID()const;

		IEntity* GetEntity()const;
	private:
		CUuid m_id;
		IEntity* m_pEntity;
	};

}
