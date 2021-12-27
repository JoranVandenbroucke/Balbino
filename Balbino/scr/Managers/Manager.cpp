#include "pch.h"
#include "Manager.h"

#include "../Input/InputHandler.h"

Balbino::CInputHandler* Balbino::CManager::s_inputHandler{nullptr};
Balbino::CManager::CManager()
= default;

Balbino::CManager::~CManager()
= default;

void Balbino::CManager::Cleanup()
{
	s_inputHandler->Cleanup();

	delete s_inputHandler;
	s_inputHandler = nullptr;
}
void Balbino::CManager::SetInputHandler(CInputHandler* pInputHandler)
{
	s_inputHandler = pInputHandler;
}

Balbino::CInputHandler* Balbino::CManager::GetInputHandler()
{
	return s_inputHandler;
}
