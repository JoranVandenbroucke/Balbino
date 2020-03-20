#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "Avatar.h"
#include <map>
#include <stdlib.h>

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


	class Button
	{
	public:
		Button();

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
		void SetName( const std::string newName )
		{
			m_Name = newName;
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

	class InputManager final: public Singleton<InputManager>
	{
	public:
		static void Init();
		static bool ProcessInput();
		static std::shared_ptr<Command> IsPressed();
		//static bool IsPressed(ControllerButton button);
	private:
		void IInit();
		bool IProcessInput();
		std::shared_ptr<Command> IIsPressed() const;
		//bool IIsPressed(ControllerButton Command ) const;

		XINPUT_STATE m_CurrentState{};
		std::unique_ptr<Button> m_Fire;
		std::unique_ptr<Button> m_Duck;
		std::unique_ptr<Button> m_Jump;
		std::unique_ptr<Button> m_Fart;
	};
}
