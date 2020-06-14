#pragma once
#include "Component.h"
#include "..\InputManager.h"

namespace Balbino
{
	class GameObject;
	class Rigidbody2D;
	class Animation;
	class BubbleManager;;

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
#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // BALBINO_DEBUG
		virtual void OnCollisionEnter( GameObject* pGameObject );
		void SetInput( const char* const device );
		bool WantsToDropDown()const;
		void GoLeft();
		void GoRight();
		void Jump();
		void Fall();
		void Shoot();
		void SetDead(bool isDead);
		bool GetDeadState();
		Direction GetDirection();
	private:
		bool m_WantsToDropDown;
		bool m_IsDead;
		Direction m_Direction;

		std::string m_CurrentDevice;

		Rigidbody2D* m_pRigidbody;
		Animation* m_pAnimation;
		BubbleManager* m_pManager;
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