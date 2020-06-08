#pragma once
#include "Singleton.h"
#include <box2d.h>
namespace Balbino
{
	class PhysicsWorld : public Balbino::Singleton<PhysicsWorld>
	{
	public:
		~PhysicsWorld();

		static void Update();
		static b2Body* AddBody( const b2BodyDef* body );
		static b2World& GetWorld();
		static void RemoveBody( b2Body* body );
	private:
		PhysicsWorld();
		friend class Singleton<PhysicsWorld>;

		void Initialize();
		void IUpdate();

		b2World m_World;
	};
}