#pragma once
#include "Component.h"

namespace Balbino
{
	class GameObject;
	class Rigidbody2D;
	class Animation;
	class BubbleManager;

	class Bubble final : public Component
	{
	public:
		explicit Bubble( const GameObject* const pOrigine );
		virtual~Bubble() = default;
		Bubble( const Bubble& ) = delete;
		Bubble( Bubble&& ) = delete;
		Bubble& operator=( const Bubble& ) = delete;
		Bubble& operator=( Bubble&& ) = delete;

		// Inherited via Component
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
		virtual void Save( std::ostream& file ) override;
		virtual void Load( std::istream& file ) override;
#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG
		void SetPosition( const Balbino::Vector2& positon );
		void SetAlive();
		bool IsAlive();
		void SetDirection( Direction direction );
		virtual void OnTriggerEnter( GameObject* pGameObject )override; ////do stuf

	private:
		float m_LifeTime;
		Rigidbody2D* m_pRigidBody;
		Animation* m_pAnimation;
		BubbleManager* m_pManager;
	};
}
