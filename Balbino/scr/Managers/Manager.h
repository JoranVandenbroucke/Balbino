#pragma once
#include "../Core.h"

namespace Balbino
{
	class CInputHandler;

	class BALBINO_API CManager
	{
	public:
		CManager();
		~CManager();
		CManager(const CManager&) = delete;
		CManager(CManager&&) = delete;
		CManager& operator=(const CManager&) = delete;
		CManager& operator=(CManager&&) = delete;

		void Cleanup();

		void SetInputHandler(CInputHandler* pInputHandler);

		static CInputHandler* GetInputHandler();
	private:
		static CInputHandler* s_inputHandler;
	};
}
