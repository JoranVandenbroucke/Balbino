#pragma once
#include "IScene.h"

namespace BalEditor
{
	class CSceneHierarchy;
	class CGameView
	{
	public:
		CGameView() = default;
		~CGameView() = default;
		CGameView( const CGameView& ) = delete;
		CGameView( CGameView&& ) = delete;
		CGameView& operator=( const CGameView& ) = delete;
		CGameView& operator=( CGameView&& ) = delete;
		void Draw();

		void SetContext( IScene* pContext, CSceneHierarchy* pSceneHierarchy );
		void SetSnap( bool snap );
		void SetGuizmo( int key );

		int GetGuizmoType() const;
	private:
		CSceneHierarchy* m_pSceneHierarchy;
		IScene* m_pContext;
		int m_gizmoType = 7;
		bool m_snap;
	};
}
