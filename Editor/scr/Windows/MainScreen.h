#pragma once
#include <string>

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

	private:
		std::string m_savePosition;
		bool m_saved = false;
	};
}