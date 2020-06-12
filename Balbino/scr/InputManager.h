#pragma once
#include "Singleton.h"
#include <windows.h>
#include <XInput.h>
#include <map>
#include <stdlib.h>

namespace Balbino
{
	class CharacterController;
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute( CharacterController* actor ) = 0;
	};

	class Button
	{
	public:
		Button();
		~Button();
		Balbino::Command* GetCommand()const;
		const std::string& GetName()const;
		void SetCommand( Command* const pNewCommand );
		void SetName( const std::string& newName );
		uint16_t GetScanCode();
	private:
		static std::map<std::string, uint16_t> m_ButtonMap;
		Command* m_pCommand{};
		std::string m_Name{};
	};

	class InputManager
	{
	public:
		static InputManager& Get();
		static void Init();
		static bool ProcessInput();
		static void ScanDevices();
		static Command* const IsPressed( const std::string& device = "keyboard" );
		static void ChangeButton( const std::string device, const std::string& functionName );
		static void ChangeButton( const std::string device, const std::string& functionName, const std::string& keyName );
		static void AddButton( const std::string& functionName, Command* command, const std::string device = "");
		static void Fini();

		static void DrawInspector();
		static std::vector<std::string> GetAllInputDevices();
		~InputManager() = default;
		InputManager( const InputManager& ) = delete;
		InputManager( InputManager&& ) = delete;
		InputManager& operator=( const InputManager& ) = delete;
		InputManager& operator=( InputManager&& ) = delete;
	private:
		void IScanDevices();
		InputManager();
		void IInit();
		bool IProcessInput();
		Command* const IIsPressed( const std::string& device = "keyboard" ) const;
		void IAddButton( const std::string& functionName, Command* command, const std::string device = "" );
		std::vector<std::string> IGetAllInputDevices();

		void IFini();
		void IDrawInspector();

		std::string DeviceToID( const std::string& device )const;
		std::string JoystickGUIDToID( const SDL_JoystickGUID& id )const;

		std::string m_Current{ "keyboard" };
		std::map<std::string,std::map<std::string, Button* const>> m_pButtons;
		const std::string m_keyboard{ "keyboard" };
		std::map<std::string, SDL_GameController*> m_ControllerList;
		std::map<std::string, SDL_Joystick*> m_JoystickList;
	};
}