#pragma once
#include <string>

struct IScene;

namespace BalEditor
{
	class CMainScreen
	{
	public:
		CMainScreen() = default;
		~CMainScreen() = default;
		CMainScreen( const CMainScreen& ) = delete;
		CMainScreen( CMainScreen&& ) = delete;
		CMainScreen& operator=( const CMainScreen& ) = delete;
		CMainScreen& operator=( CMainScreen&& ) = delete;

		void Draw();
		void SetContext( IScene* pScene );

	private:
		std::string m_savePosition;
		bool m_saved = false;

		IScene* m_pContext;
	};
}
