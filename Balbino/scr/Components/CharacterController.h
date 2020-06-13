#pragma once
#include "Component.h"
#include "..\InputManager.h"

namespace Balbino
{
	class GameObject;
	class Rigidbody2D;
	class Animation;

	class CharacterController : public Component
	{
	public:
		explicit CharacterController( const GameObject* const origine );
		~CharacterController()=default;
		CharacterController( const CharacterController& ) = delete;
		CharacterController( CharacterController&& ) = delete;
		CharacterController& operator=( const CharacterController& ) = delete;
		CharacterController& operator=( CharacterController&& ) = delete;

		// Inherited via Component
		virtual void Create() override;
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void Draw() const override;
		virtual void Save( std::ostream& file ) override;
		virtual void Load( std::istream& file ) override;
		virtual void DrawInpector() override;

		void SetInput( const char* const device );

		void GoLeft();
		void GoRight();
		void LookLeft();
		void LookRight();
		void Jump();
		void Fall();
		void Shoot();
	private:
		std::string m_CurrentDevice;

		Rigidbody2D* m_pRigidbody;
		Animation* m_pAnimation;
		bool m_IsJumping;
	};

	class GoLeftCommand : public Command
	{
	public:
		// Inherited via Command
		virtual void Execute( CharacterController* actor ) override;
	};
	class GoRightCommand : public Command
	{
	public:
		// Inherited via Command
		virtual void Execute( CharacterController* actor ) override;
	};
	class JumpCommand : public Command
	{
	public:
		// Inherited via Command
		virtual void Execute( CharacterController* actor ) override;
	};
	class FallCommand : public Command
	{
	public:
		// Inherited via Command
		virtual void Execute( CharacterController* actor ) override;
	};
	class ShootCommand : public Command
	{
	public:
		// Inherited via Command
		virtual void Execute( CharacterController* actor ) override;
	};
}