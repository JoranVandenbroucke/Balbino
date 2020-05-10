#pragma once
#include "Singleton.h"
#include <box2d/box2d.h>
namespace Balbino
{
	class PhysicsWorld : public Balbino::Singleton<PhysicsWorld>
	{
	public:
		~PhysicsWorld();

		static void Update();
		static b2Body* AddBoddy( const b2BodyDef* body );
		static b2World& GetWorld();
	private:
		PhysicsWorld();
		friend class Singleton<PhysicsWorld>;

		void Initialize();
		void IUpdate();

		b2World m_World;
	};
}