#include "BalbinoPCH.h"
#include "InputManager.h"
#include <SDL.h>

std::map<std::string, uint16_t> Balbino::Button::m_ButtonMap{};

Balbino::Button::Button()
{
	m_ButtonMap.insert( std::make_pair( "C_A", uint16_t( XINPUT_GAMEPAD_A ) ) );
	m_ButtonMap.insert( std::make_pair( "C_B", uint16_t( XINPUT_GAMEPAD_B ) ) );
	m_ButtonMap.insert( std::make_pair( "C_X", uint16_t( XINPUT_GAMEPAD_X ) ) );
	m_ButtonMap.insert( std::make_pair( "C_Y", uint16_t( XINPUT_GAMEPAD_Y ) ) );

	m_ButtonMap.insert( std::make_pair( "C_D_UP", uint16_t( XINPUT_GAMEPAD_DPAD_UP ) ) );
	m_ButtonMap.insert( std::make_pair( "C_D_DOWN", uint16_t( XINPUT_GAMEPAD_DPAD_DOWN ) ) );
	m_ButtonMap.insert( std::make_pair( "C_D_LEFT", uint16_t( XINPUT_GAMEPAD_DPAD_LEFT ) ) );
	m_ButtonMap.insert( std::make_pair( "C_D_RIGHT", uint16_t( XINPUT_GAMEPAD_DPAD_RIGHT ) ) );

	m_ButtonMap.insert( std::make_pair( "C_START", uint16_t( XINPUT_GAMEPAD_START ) ) );
	m_ButtonMap.insert( std::make_pair( "C_BACK", uint16_t( XINPUT_GAMEPAD_BACK ) ) );

	m_ButtonMap.insert( std::make_pair( "C_L_THUMB", uint16_t( XINPUT_GAMEPAD_LEFT_THUMB ) ) );
	m_ButtonMap.insert( std::make_pair( "C_R_THUMB", uint16_t( XINPUT_GAMEPAD_RIGHT_THUMB ) ) );

	m_ButtonMap.insert( std::make_pair( "C_L_THUMB", uint16_t( XINPUT_GAMEPAD_LEFT_SHOULDER ) ) );
	m_ButtonMap.insert( std::make_pair( "C_R_THUMB", uint16_t( XINPUT_GAMEPAD_RIGHT_SHOULDER ) ) );
}

void Balbino::InputManager::Init()
{
	GetInstance().IInit();
}

bool Balbino::InputManager::ProcessInput()
{
	return GetInstance().IProcessInput();
}

std::shared_ptr<Balbino::Command> Balbino::InputManager::IsPressed()
{
	return GetInstance().IIsPressed();
}

void Balbino::InputManager::IInit()
{
	m_Fire = std::make_unique<Button>();
	m_Fire->SetName( "C_A" );
	m_Fire->SetCommand( std::make_shared<Fire>() );

	m_Duck = std::make_unique<Button>();
	m_Duck->SetName( "C_B" );
	m_Duck->SetCommand( std::make_shared<Duck>() );

	m_Jump = std::make_unique<Button>();
	m_Jump->SetName( "C_X" );
	m_Jump->SetCommand( std::make_shared<Jump>() );

	m_Fart = std::make_unique<Button>();
	m_Fart->SetName( "C_Y" );
	m_Fart->SetCommand( std::make_shared<Fart>() );
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

std::shared_ptr<Balbino::Command> Balbino::InputManager::IIsPressed() const
{
	if( m_Fire->GetName()[0] == 'C' && m_CurrentState.Gamepad.wButtons & m_Fire->GetKeyCode() )
	{
		return m_Fire->GetCommand();
	}
	else
	{

	}
	if( m_Duck->GetName()[0] == 'C' && m_CurrentState.Gamepad.wButtons & m_Duck->GetKeyCode() )
	{
		return m_Duck->GetCommand();
	}
	else
	{

	}
	if( m_Jump->GetName()[0] == 'C' && m_CurrentState.Gamepad.wButtons & m_Jump->GetKeyCode() )
	{
		return m_Jump->GetCommand();
	}
	else
	{

	}
	if( m_Fart->GetName()[0] == 'C' && m_CurrentState.Gamepad.wButtons & m_Fart->GetKeyCode() )
	{
		return m_Fart->GetCommand();
	}
	else
	{

	}
	return nullptr;
}