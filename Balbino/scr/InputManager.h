#pragma once
#include <windows.h>
#include <XInput.h>
#include "Singleton.h"
#include <map>
#include <stdlib.h>
#include "Components/Avatar.h"


namespace Balbino
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute( Avatar& actor ) = 0;
	};

	class Fire: public Command
	{
	public:
		virtual void Execute( Avatar& actor )override
		{
			actor.Fire();
		}
	};
	class Duck: public Command
	{
	public:
		virtual void Execute( Avatar& actor )override
		{
			actor.Duck();
		}
	};
	class Jump: public Command
	{
	public:
		virtual void Execute( Avatar& actor )override
		{
			actor.Jump();
		}
	};
	class Fart: public Command
	{
	public:
		virtual void Execute( Avatar& actor )override
		{
			actor.Fart();
		}
	};

	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		//...
	};
	class Button
	{
	public:
		Button();
		~Button();
		const std::shared_ptr<Command> GetCommand()const
		{
			return m_Command;
		}
		const std::string& GetName()const
		{
			return m_Name;
		}
		void SetCommand( const std::shared_ptr<Command> newCommand )
		{
			m_Command = newCommand;
		}
		void SetName( const std::string& newName )
		{
			if( m_ButtonMap.find( newName ) != m_ButtonMap.end() )
			{
				m_Name = newName;
			}
		}
		uint16_t GetKeyCode()
		{
			return m_ButtonMap[m_Name];
		};
	private:
		static std::map<std::string, uint16_t> m_ButtonMap;
		std::shared_ptr<Command> m_Command{};
		std::string m_Name{};
	};

	class InputManager
	{
	public:
		static InputManager& Get();
		static void Init();
		static bool ProcessInput();
		static std::shared_ptr<Command> IsPressed();
		static bool IsPressed( const ControllerButton& button );
		static void ChangeButton( const std::string& functionName );
		static void ChangeButton( const std::string& functionName, const std::string& keyName );

		~InputManager() = default;
		InputManager( const InputManager& ) = delete;
		InputManager( InputManager&& ) = delete;
		InputManager& operator=( const InputManager& ) = delete;
		InputManager& operator=( InputManager&& ) = delete;
	private:
		InputManager() = default;
		void IInit();
		bool IProcessInput();
		std::shared_ptr<Command> IIsPressed() const;
		bool IIsPressed( const ControllerButton& button ) const;

		XINPUT_STATE m_CurrentState{};
		std::shared_ptr<Button> m_Fire;
		std::shared_ptr<Button> m_Duck;
		std::shared_ptr<Button> m_Jump;
		std::shared_ptr<Button> m_Fart;
	};
}