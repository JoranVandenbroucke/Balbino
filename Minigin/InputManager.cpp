#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool Balbino::InputManager::ProcessInput()
{
	return GetInstance().IProcessInput();
}

bool Balbino::InputManager::IsPressed(ControllerButton button)
{
	return GetInstance().IIsPressed( button );
}

bool Balbino::InputManager::IProcessInput()
{
	ZeroMemory( &m_CurrentState, sizeof( XINPUT_STATE ) );
	XInputGetState( 0, &m_CurrentState );

	SDL_Event e;
	while( SDL_PollEvent( &e ) )
	{
		if( e.type == SDL_QUIT )
		{
			return false;
		}
		if( e.type == SDL_KEYDOWN )
		{

		}
		if( e.type == SDL_MOUSEBUTTONDOWN )
		{

		}
	}
	return true;
}

bool Balbino::InputManager::IIsPressed( ControllerButton button ) const
{
	switch( button )
	{
		case ControllerButton::ButtonA:
			return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
		case ControllerButton::ButtonB:
			return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
		case ControllerButton::ButtonX:
			return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
		case ControllerButton::ButtonY:
			return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
		default: return false;
	}
}

