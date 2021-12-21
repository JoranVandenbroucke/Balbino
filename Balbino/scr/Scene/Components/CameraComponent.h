#pragma once
#include "../Camera.h"

struct IEntity;

namespace Balbino
{
	class CCameraComponent final
	{
	public:
		CCameraComponent( IEntity* pEntity );
		~CCameraComponent()=default;
		CCameraComponent( const CCameraComponent& ) = default;
		CCameraComponent( CCameraComponent&& ) = default;
		CCameraComponent& operator=( const CCameraComponent& ) = default;
		CCameraComponent& operator=( CCameraComponent&& ) = default;

		void SetCamera( const CCamera& Camera );
		void SetIsPrimary( bool isPrimary );

		CCamera& GetCamera();
		[[nodiscard]] const CCamera& GetCamera()const;
		[[nodiscard]] bool GetIsPrimary()const;

		IEntity* GetEntity()const;
	private:
		bool m_primary;
		CCamera m_camera;
		IEntity* m_pEntity;
	};
}
