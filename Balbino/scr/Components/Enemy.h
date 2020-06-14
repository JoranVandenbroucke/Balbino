#pragma once
#include "Component.h"
#include <random>

namespace Balbino
{
	class GameObject;
	class Rigidbody2D;
	class Animation;
	class Enemy : public Component
	{
	public:
		explicit Enemy( const GameObject* const origin );
		virtual ~Enemy() = default;
		Enemy( const Enemy& ) = delete;
		Enemy( Enemy&& ) = delete;
		Enemy operator=( const Enemy& ) = delete;
		Enemy operator=( Enemy&& ) = delete;

		enum class EnemyType
		{
			ZenChan,
			Hidegons,
			Banebou,
			Pulpul,
			Monsta,
			Drunk,
			Maita,
			Invader
		};

		// Inherited via Component
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
		virtual void Save( std::ostream& file ) override;
		virtual void Load( std::istream& file ) override;
#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // BALBINO_DEBUG

		void SetPositon( unsigned char collum, unsigned char row );
		void SetAnimationOffset( unsigned char offset );
		void SetSpawnDelay( float delay );
		bool WantsToDropDown()const;
		void SetType( EnemyType type );
	private:
		EnemyType m_EnemyType;
		std::vector<Component*> m_pAllcomponents;
		Rigidbody2D* m_pRigidBody;
		Animation* m_pAnimation;
		char m_Action;
		float m_SpawnDleay;
		float m_ChaingeMovetentTimer;
		const float m_MaxMovementTime{ 3.f };
		std::random_device m_RandomDevice;
		std::mt19937 m_Generator;
		std::uniform_int_distribution<> m_Destribution;
		//trying black magic
		class State
		{
		public:
			virtual ~State()
			{
			}
			virtual State* Update( Enemy& enemy ) = 0;
		};
		class IdleState :public State
		{
		public:
			IdleState()
				:State()
			{
			}
			// Inherited via State
			virtual State* Update( Enemy& enemy ) override;
		};
		class LeftState :public State
		{
		public:
			LeftState()
				:State()
			{
			}
			// Inherited via State
			virtual State* Update( Enemy& enemy ) override;
		};
		class RightState :public State
		{
		public:
			RightState()
				:State()
			{
			}
			// Inherited via State
			virtual State* Update( Enemy& enemy ) override;
		};
		class InAirState :public State
		{
		public:
			InAirState()
				:State()
			{
			}
			// Inherited via State
			virtual State* Update( Enemy& enemy ) override;
		};
		class JumpState :public State
		{
		public:
			JumpState()
				:State()
			{
			}
			// Inherited via State
			virtual State* Update( Enemy& enemy ) override;
		};
		class GoDownState :public State
		{
		public:
			GoDownState()
				:State()
			{
			}
			// Inherited via State
			virtual State* Update( Enemy& enemy ) override;
		};
		State* m_pCurrentState;
		bool m_WantToGoDawn;
	};
}

