#pragma once

namespace BalEditor
{
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
	};
}
