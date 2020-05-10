#include "BalbinoPCH.h"
#include "InputManager.h"
#include <SDL.h>

#include "imgui-1.75/imgui.h"
#include "imgui-1.75/imgui_impl_sdl.h"

//#include <SDL_opengl.h>
//#include "Application.h"

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

	for( int scan = SDL_SCANCODE_UNKNOWN; scan <= SDL_NUM_SCANCODES; scan++ )
	{
		SDL_Keycode code = SDL_GetKeyFromScancode( SDL_Scancode( scan ) );
		if( code != SDLK_UNKNOWN )
		{
			const char* name = SDL_GetKeyName( code );
			m_ButtonMap.insert( std::make_pair( name, uint16_t( scan ) ) );
		}
	}
}

Balbino::Button::~Button()
{
	m_ButtonMap.clear();
}

Balbino::InputManager::InputManager()
{
}

Balbino::InputManager& Balbino::InputManager::Get()
{
	static InputManager instance{};
	return instance;
}

void Balbino::InputManager::Init()
{
	Get().IInit();
}

bool Balbino::InputManager::ProcessInput()
{
	return Get().IProcessInput();
}

Balbino::Command* const Balbino::InputManager::IsPressed()
{
	return Get().IIsPressed();
}

bool Balbino::InputManager::IsPressed( const ControllerButton& button )
{
	return Get().IIsPressed( button );
}

void Balbino::InputManager::ChangeButton( const std::string& functionName )
{
	SDL_Event e;
	while( true )
	{
		if( SDL_WaitEvent( &e ) )
		{
			if( e.type == SDL_KEYDOWN )
			{
				SDL_KeyboardEvent& keyEvent = e.key;
				if( keyEvent.keysym.sym == SDLK_ESCAPE )
				{
					break;
				}
				const char* name = SDL_GetKeyName( keyEvent.keysym.sym );
				ChangeButton( functionName, name );
				break;
			}
		}
	}

}

void Balbino::InputManager::ChangeButton( const std::string& functionName, const std::string& keyName )
{
	if( Get().m_pButtons.find( functionName ) != Get().m_pButtons.cend() )
	{
		Get().m_pButtons[functionName]->SetName( keyName );
	}
}

Balbino::InputManager::~InputManager()
{
	m_pButtons.clear();
}

void Balbino::InputManager::IInit()
{
	m_pButtons.emplace( std::pair{ "Fire", new Button{} } );
	m_pButtons["Fire"]->SetName( "C_A" );
	m_pButtons["Fire"]->SetCommand( new Fire{} );

	m_pButtons.emplace( std::pair{ "Duck", new Button{} } );
	m_pButtons["Duck"]->SetName( "C_B" );
	m_pButtons["Duck"]->SetCommand( new Duck{} );

	m_pButtons.emplace( std::pair{ "Jump", new Button{} } );
	m_pButtons["Jump"]->SetName( "C_X" );
	m_pButtons["Jump"]->SetCommand( new Jump{} );

	m_pButtons.emplace( std::pair{ "Fart", new Button{} } );
	m_pButtons["Fart"]->SetName( "C_Y" );
	m_pButtons["Fart"]->SetCommand( new Fart{} );

}

bool Balbino::InputManager::IProcessInput()
{
	ZeroMemory( &m_CurrentState, sizeof( XINPUT_STATE ) );
	XInputGetState( 0, &m_CurrentState );
	ImGuiIO& io = ImGui::GetIO();

	int wheel = 0;
	SDL_Event e;
	while( SDL_PollEvent( &e ) )
	{
		ImGui_ImplSDL2_ProcessEvent( &e );
		if( e.type == SDL_QUIT )
		{
			return false;
		}
		else if( e.type == SDL_KEYDOWN )
		{

		}
		else if( e.type == SDL_KEYUP )
		{

		}
		else if( e.type == SDL_MOUSEBUTTONUP )
		{
			if( e.button.button == 1 )
			{
				io.MouseDown[0] = 0;
			}
			else if( e.button.button == 2 )
			{
				io.MouseDown[1] = 0;
			}
		}
		else if( e.type == SDL_MOUSEBUTTONDOWN )
		{
			if( e.button.button == 1 )
			{
				io.MouseDown[0] = 1;
			}
			else if( e.button.button == 2 )
			{
				io.MouseDown[1] = 1;
			}
		}
		else if( e.type == SDL_MOUSEWHEEL )
		{
			wheel = e.wheel.y;
		}
		else if( e.type == SDL_MOUSEMOTION )
		{
			io.MousePos = ImVec2{ float( e.motion.x ),float( e.motion.y ) };
		}
	}
	io.MouseWheel = static_cast<float>( wheel );
	return true;
}

Balbino::Command* const Balbino::InputManager::IIsPressed() const
{
	return nullptr;
}

bool Balbino::InputManager::IIsPressed( const ControllerButton& button ) const
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
