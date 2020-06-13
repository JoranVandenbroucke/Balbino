#include "BalbinoPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <SDL_gamecontroller.h>

#include "imgui-1.75/imgui.h"
#include "imgui-1.75/imgui_impl_sdl.h"
#include <regex>
#include <fstream>
//#include <SDL_opengl.h>
//#include "Application.h"

std::map<std::string, uint16_t> Balbino::Button::m_ButtonMap{};

Balbino::Button::Button()
{
	//m_ButtonMap.insert( std::make_pair( "C_A", uint16_t( XINPUT_GAMEPAD_A ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_B", uint16_t( XINPUT_GAMEPAD_B ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_X", uint16_t( XINPUT_GAMEPAD_X ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_Y", uint16_t( XINPUT_GAMEPAD_Y ) ) );

	//m_ButtonMap.insert( std::make_pair( "C_D_UP", uint16_t( XINPUT_GAMEPAD_DPAD_UP ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_D_DOWN", uint16_t( XINPUT_GAMEPAD_DPAD_DOWN ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_D_LEFT", uint16_t( XINPUT_GAMEPAD_DPAD_LEFT ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_D_RIGHT", uint16_t( XINPUT_GAMEPAD_DPAD_RIGHT ) ) );

	//m_ButtonMap.insert( std::make_pair( "C_START", uint16_t( XINPUT_GAMEPAD_START ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_BACK", uint16_t( XINPUT_GAMEPAD_BACK ) ) );

	//m_ButtonMap.insert( std::make_pair( "C_L_THUMB", uint16_t( XINPUT_GAMEPAD_LEFT_THUMB ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_R_THUMB", uint16_t( XINPUT_GAMEPAD_RIGHT_THUMB ) ) );

	//m_ButtonMap.insert( std::make_pair( "C_L_THUMB", uint16_t( XINPUT_GAMEPAD_LEFT_SHOULDER ) ) );
	//m_ButtonMap.insert( std::make_pair( "C_R_THUMB", uint16_t( XINPUT_GAMEPAD_RIGHT_SHOULDER ) ) );

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
	if( m_pCommand != m_pCommand )
	{
		delete m_pCommand;
	}
	m_pCommand = nullptr;
	m_ButtonMap.clear();
}

Balbino::Button::Button( const Button& other )
{
	if( &other != this )
	{
		this->m_Name = other.m_Name;
		this->m_pCommand = other.m_pCommand;
	}
}

Balbino::Command* Balbino::Button::GetCommand() const
{
	return m_pCommand;
}

const std::string& Balbino::Button::GetName() const
{
	return m_Name;
}

void Balbino::Button::SetCommand( Command* const pNewCommand )
{
	m_pCommand = pNewCommand;
}

void Balbino::Button::SetName( const std::string& newName )
{
	if( m_ButtonMap.find( newName ) != m_ButtonMap.end() )
	{
		m_Name = newName;
	}
	else
	{
		auto button = SDL_GameControllerGetButtonFromString( newName.c_str() );
		if( button != SDL_CONTROLLER_BUTTON_INVALID )
		{
			m_ButtonMap.insert( std::pair( newName, uint16_t( button ) ) );
			m_Name = newName;
		}
	}
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

uint16_t Balbino::Button::GetScanCode()
{
	return m_ButtonMap[m_Name];
}

Balbino::InputManager::InputManager()
{
}

bool Balbino::InputManager::ProcessInput()
{
	return Get().IProcessInput();
}

void Balbino::InputManager::ScanDevices()
{
	Get().IScanDevices();
}

Balbino::Command* const Balbino::InputManager::IsPressed( const std::string& device )
{
	return Get().IIsPressed( device );
}

void Balbino::InputManager::ChangeButton( const std::string device, const std::string& functionName )
{
	std::string id = Get().DeviceToID( device );
	SDL_Event e;
	while( true )
	{
		if( SDL_WaitEvent( &e ) )
		{
			if( device == Get().m_Keyboard && e.type == SDL_KEYDOWN )
			{
				SDL_KeyboardEvent& keyEvent = e.key;
				if( keyEvent.keysym.sym == SDLK_ESCAPE )
				{
					break;
				}
				const char* name = SDL_GetKeyName( keyEvent.keysym.sym );
				ChangeButton( device, functionName, name );
				break;
			}
			if( e.type == SDL_CONTROLLERBUTTONDOWN )
			{
				SDL_ControllerButtonEvent controllerEvent = e.cbutton;
				if( Get().m_ControllerList.find( device ) != Get().m_ControllerList.end() && SDL_GameControllerGetButton( Get().m_ControllerList.at( device ), SDL_GameControllerButton( controllerEvent.button ) ) )
				{
					const char* name = SDL_GameControllerGetStringForButton( SDL_GameControllerButton( controllerEvent.button ) );
					ChangeButton( device, functionName, name );
				}
				break;
			}
		}
	}

}

void Balbino::InputManager::ChangeButton( const std::string device, const std::string& functionName, const std::string& keyName )
{
	std::string uID = Get().DeviceToID( device );;
	if( Get().m_pButtons.find( uID ) != Get().m_pButtons.cend() )
		if( Get().m_pButtons[uID].find( functionName ) != Get().m_pButtons[uID].end() )
			Get().m_pButtons[uID][functionName]->SetName( keyName );
}

void Balbino::InputManager::AddButton( const std::string& functionName, Command* command, const std::string device )
{
	Get().IAddButton( functionName, command, device );
}

void Balbino::InputManager::Fini()
{
	Get().IFini();
}

void Balbino::InputManager::DrawInspector()
{
	Get().IDrawInspector();
}

std::vector<std::string> Balbino::InputManager::GetAllInputDevices()
{
	return Get().IGetAllInputDevices();
}

void Balbino::InputManager::IScanDevices()
{
	const char* extention{ ".bcc" };
	std::string fileName{ "ButtonProfiles/" };
	fileName += m_Keyboard;
	fileName += extention;
	std::transform( fileName.begin(), fileName.end(), fileName.begin(), []( char ch )
	{
		return ch == ' ' ? '_' : ch;
	} );
	std::ifstream file{ fileName };
	if( file.is_open() )
	{
		std::string line{};
		while( std::getline( file, line ) )
		{
			std::regex functionKeyName{ R"(^([a-zA-Z]+):([a-zA-Z ]+)$)" };
			std::smatch match;
			if( std::regex_match( line, match, functionKeyName ) )
			{
				ChangeButton( m_Keyboard, match[1].str(), match[2].str() );
			}
		}
	}
	std::string uID = JoystickGUIDToID( SDL_JoystickGUID{ Uint8( -1 ) } );
	std::map<std::string, Button* const> keyboard = m_pButtons[uID];
	for( auto controller : m_ControllerList )
	{
		SDL_GameControllerClose( controller.second );
	}
	for( auto joystick : m_JoystickList )
	{
		SDL_JoystickClose( joystick.second );
	}
	m_ControllerList.clear();
	m_JoystickList.clear();

	m_pButtons.clear();
	m_pButtons.insert( std::make_pair( uID, keyboard ) );
	for( int i = 0; i < SDL_NumJoysticks(); ++i )
	{
		if( SDL_IsGameController( i ) )
		{
			SDL_GameController* controller = nullptr;
			controller = SDL_GameControllerOpen( i );
			if( controller )
			{
				uID = JoystickGUIDToID( SDL_JoystickGetGUID( SDL_GameControllerGetJoystick( controller ) ) );
				std::string deviceName = SDL_GameControllerName( controller );
				m_ControllerList.insert( std::make_pair( SDL_GameControllerName( controller ), controller ) );

				decltype( keyboard ) boards{};
				for( auto key : keyboard )
				{
					boards.insert( std::make_pair(key.first, new Button(*key.second)) );
				}
				m_pButtons.insert( std::make_pair( uID, boards ) );

				//const char* extention{ ".bcc" };
				fileName = "ButtonProfiles/";
				fileName += deviceName;
				fileName += extention;
				std::transform( fileName.begin(), fileName.end(), fileName.begin(), []( char ch )
				{
					return ch == ' ' ? '_' : ch;
				} );
				std::ifstream controllerFile{ fileName };
				if( controllerFile.is_open() )
				{
					std::string line{};
					while( std::getline( controllerFile, line ) )
					{
						std::regex functionKeyName{ R"(^([a-zA-Z]+):([a-zA-Z ]+)$)" };
						std::smatch match;
						if( std::regex_match( line, match, functionKeyName ) )
						{
							ChangeButton( deviceName, match[1].str(), match[2].str() );
						}
					}
				}
			}
			else
				fprintf( stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError() );
		}
		else
		{
			SDL_Joystick* joystick = nullptr;
			joystick = SDL_JoystickOpen( i );
			if( joystick )
			{
				uID = JoystickGUIDToID( SDL_JoystickGetGUID( joystick ) );
				std::string deviceName = SDL_JoystickName( joystick );
				m_JoystickList.insert( std::make_pair( deviceName.c_str(), joystick ) );
				m_pButtons.insert( std::make_pair( uID, keyboard ) );

				fileName = "ButtonProfiles/";
				fileName += deviceName;
				fileName += extention;
				std::transform( fileName.begin(), fileName.end(), fileName.begin(), []( char ch )
				{
					return ch == ' ' ? '_' : ch;
				} );
				std::ifstream joystickFile{ fileName };
				if( joystickFile.is_open() )
				{
					std::string line{};
					while( std::getline( joystickFile, line ) )
					{
						std::regex functionKeyName{ R"(^([a-zA-Z]+):([a-zA-Z ]+)$)" };
						std::smatch match;
						if( std::regex_match( line, match, functionKeyName ) )
						{
							ChangeButton( deviceName, match[1].str(), match[2].str() );
						}
					}
				}
			}
			else
				fprintf( stderr, "Could not open Joistick %i: %s\n", i, SDL_GetError() );
		}
	}
}

void Balbino::InputManager::IInit()
{
	IScanDevices();
}

bool Balbino::InputManager::IProcessInput()
{
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
		else if( e.type == SDL_CONTROLLERBUTTONDOWN )
		{
		}
		else if( e.type == SDL_CONTROLLERBUTTONUP )
		{
		}
		else if( e.type == SDL_CONTROLLERDEVICEADDED )
		{
			IScanDevices();
		}
		else if( e.type == SDL_CONTROLLERDEVICEREMOVED )
		{
			IScanDevices();
		}
	}
	io.MouseWheel = static_cast<float>( wheel );
	return true;
}

Balbino::Command* const Balbino::InputManager::IIsPressed( const std::string& device ) const
{
	std::string uID = DeviceToID( device );

	if( device == m_Keyboard )
	{
		const Uint8* pStates = SDL_GetKeyboardState( nullptr );
		for( auto button : m_pButtons.at( uID ) )
			if( pStates[button.second->GetScanCode()] )
				return button.second->GetCommand();
	}
	else
	{
		if( m_ControllerList.find( device.c_str() ) != m_ControllerList.end() )
		{
			for( auto button : m_pButtons.at( uID ) )
			{
				if( SDL_GameControllerGetButton( m_ControllerList.at( device.c_str() ), SDL_GameControllerButton( button.second->GetScanCode() ) ) )
					return button.second->GetCommand();
			}
		}
		else if( m_JoystickList.find( device.c_str() ) != m_JoystickList.end() )
		{
			for( auto button : m_pButtons.at( uID ) )
			{
				if( SDL_JoystickGetButton( m_JoystickList.at( device.c_str() ), SDL_GameControllerButton( button.second->GetScanCode() ) ) )
					return button.second->GetCommand();
			}
		}
	}
	return nullptr;
}

void Balbino::InputManager::IAddButton( const std::string& functionName, Command* command, const std::string device )
{
	std::string uID = DeviceToID( device );
	if( m_pButtons[uID].find(functionName) != m_pButtons[uID].end() )
		return;

	if( device == "" )
	{
		for( auto buttonMap : m_pButtons )
		{
			uID = buttonMap.first;
			m_pButtons[uID].insert( std::make_pair( functionName, new Button{} ) );
			m_pButtons[uID][functionName]->SetCommand( command );
			m_pButtons[uID][functionName]->SetName( "Space" );
		}
	}
	else
	{
		m_pButtons[uID].insert( std::make_pair( functionName, new Button{} ) );
		m_pButtons[uID][functionName]->SetCommand( command );
		m_pButtons[uID][functionName]->SetName( "Space" );
	}
	ScanDevices();
}

std::vector<std::string> Balbino::InputManager::IGetAllInputDevices()
{
	int deviceNr{};
	std::vector<std::string> devices{ std::to_string( ++deviceNr ) + ": " + m_Keyboard };
	for( auto controller : m_ControllerList )
	{
		devices.push_back( std::to_string( ++deviceNr ) + ": " + controller.first );
	}
	for( auto joystick : m_JoystickList )
	{
		devices.push_back( std::to_string( ++deviceNr ) + ": " + joystick.first );
	}
	return devices;
}

void Balbino::InputManager::IDrawInspector()
{
	auto old = std::string( m_Current );
	ImGui::Begin( "Input Manager" );
	if( ImGui::BeginCombo( "input", m_Current.c_str() ) )
	{
		bool isSelected{ m_Current == m_Keyboard };
		if( ImGui::Selectable( m_Keyboard.c_str() ) )
		{
			m_Current = m_Keyboard;
		}
		if( isSelected )
			ImGui::SetItemDefaultFocus();
		for( auto controller : m_ControllerList )
		{
			isSelected = m_Current == controller.first;
			if( ImGui::Selectable( controller.first.c_str() ) )
			{
				m_Current = controller.first.c_str();
			}
			if( isSelected )
				ImGui::SetItemDefaultFocus();
		}
		for( auto joystick : m_JoystickList )
		{
			isSelected = m_Current == joystick.first;
			if( ImGui::Selectable( joystick.first.c_str() ) )
			{
				m_Current = joystick.first;
			}
			if( isSelected )
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	std::string uID = DeviceToID( m_Current );

	for( auto button : m_pButtons[uID] )
	{
		ImGui::PushID( button.first.c_str() );
		ImGui::Text( button.first.c_str() ); ImGui::SameLine();
		if( ImGui::Button( button.second->GetName().c_str(), { -1,64 } ) )
		{
			InputManager::ChangeButton( m_Current, button.first );
			const char* extention{ ".bcc" };
			std::string fileName{ "ButtonProfiles/" };
			fileName += m_Current;
			fileName += extention;
			std::transform( fileName.begin(), fileName.end(), fileName.begin(), []( char ch )
			{
				return ch == ' ' ? '_' : ch;
			} );
			std::ofstream file{ fileName };
			if( file.is_open() )
			{
				for( auto savinButton : m_pButtons[uID] )
				{
					file << savinButton.first << ':' << savinButton.second->GetName() << '\n';
				}
			}
		}
		ImGui::PopID();
	}
	ImGui::End();
}

std::string Balbino::InputManager::DeviceToID( const std::string& device )const
{
	SDL_JoystickGUID id{ Uint8( -1 ) };

	if( m_ControllerList.find( device ) != m_ControllerList.end() )
		id = SDL_JoystickGetGUID( SDL_GameControllerGetJoystick( m_ControllerList.at( device ) ) );
	else if( m_JoystickList.find( device ) != m_JoystickList.end() )
		id = SDL_JoystickGetGUID( m_JoystickList.at( device ) );

	return JoystickGUIDToID( id );
}

std::string Balbino::InputManager::JoystickGUIDToID( const SDL_JoystickGUID& id )const
{
	if( id.data[0] == 255 )
	{
		return std::string{ -1 };
	}
	std::string sID{};
	for( int i = 0; i < 16; i++ )
	{
		sID += std::to_string( id.data[i] );
	}
	return sID;
}

void Balbino::InputManager::IFini()
{
	for( auto& device : m_pButtons )
	{
		for( auto& button : device.second )
		{
			std::string noDevice{ -1 };
			if( device.first != noDevice )
				button.second->SetCommand( nullptr );
			delete button.second;
		}
	}
	m_pButtons.clear();
	for( auto controller : m_ControllerList )
	{
		SDL_GameControllerClose( controller.second );
	}
	for( auto joystick : m_JoystickList )
	{
		SDL_JoystickClose( joystick.second );
	}
	m_ControllerList.clear();
	m_JoystickList.clear();
}
