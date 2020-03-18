#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "Avatar.h"

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
		virtual void Execute( Balbino::Avatar& actor )override
		{
			actor.Fire();
		}
	};
	class Duck: public Command
	{
		virtual void Execute( Balbino::Avatar& actor )override
		{
			actor.Duck();
		}
	};
	class Jump: public Command
	{
		virtual void Execute( Balbino::Avatar& actor )override
		{
			actor.Jump();
		}
	};
	class Fart: public Command
	{
		virtual void Execute( Balbino::Avatar& actor )override
		{
			actor.Fart();
		}
	};

	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		static bool ProcessInput();
		static bool IsPressed(ControllerButton button);
	private:
		bool IProcessInput();
		bool IIsPressed(ControllerButton button) const;

		XINPUT_STATE m_CurrentState{};
		std::unique_ptr<Command> m_Fire;
		std::unique_ptr<Command> m_Duck;
		std::unique_ptr<Command> m_Jump;
		std::unique_ptr<Command> m_Fart;
	};
}
