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
		static bool Raycast2D( const Balbino::Vector2& origine, const Balbino::Vector2& direction, Balbino::RaycastHit& hitInfo, float maxDistance = std::numeric_limits<float>::infinity() );
	private:
		PhysicsWorld();
		friend class Singleton<PhysicsWorld>;
		void Initialize();
		void IUpdate();

		b2World m_World;

		class CollisionCallback final : public b2ContactListener
		{
		public:
			void BeginContact( b2Contact* contact );	//Called when two fixtures begin to touch.
			void EndContact( b2Contact* contact );		//Called when two fixtures cease to touch.
			//based on https://www.iforce2d.net/b2dtut/one-way-walls
			void PreSolve( b2Contact* contact, const b2Manifold* oldManifold );
		}m_CollisionCallback;

		class RaysCastCallback : public b2RayCastCallback
		{
		public:
			RaysCastCallback()
				: m_Fixture( nullptr )
			{
			}
			float ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
			{
				m_Fixture = fixture;
				m_Point = point;
				m_Normal = normal;
				m_Fraction = fraction;
				return fraction;
			}
			b2Fixture* GetFixture()const
			{
				return m_Fixture;
			};
			const b2Vec2& GetPoint()const
			{
				return m_Point;
			};
			const b2Vec2& GetNormal()const
			{
				return m_Normal;
			};
			float GetFraction()const
			{
				return m_Fraction;
			};
		private:
			b2Fixture* m_Fixture;
			b2Vec2 m_Point;
			b2Vec2 m_Normal;
			float m_Fraction;
		};
	};
}