#pragma once
#include "Component.h"
#include "Rigidbody2D.h"
#include <box2d.h>

namespace Balbino
{
	class Collider2D;
	//https://docs.unity3d.com/ScriptReference/Rigidbody.html
	//https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md55
	class Rigidbody2D : public Component
	{
	public:
		explicit Rigidbody2D( const GameObject* const origine );
		virtual ~Rigidbody2D() = default;

		Rigidbody2D( const Rigidbody2D& ) = delete;
		Rigidbody2D( Rigidbody2D&& ) = delete;
		Rigidbody2D& operator=( const Rigidbody2D& ) = delete;
		Rigidbody2D& operator=( Rigidbody2D&& ) = delete;

		virtual void Create() override;
		virtual void FixedUpdate()override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file ) override;
		virtual void Load( std::istream& file ) override;

		bool SweepTest( const Balbino::Vector3& direction, Balbino::RaycastHit& hitInfo, float maxDistance = std::numeric_limits<float>::infinity() ); //this is not working yet
		Balbino::RaycastHit& SweepTestAll( const Balbino::Vector3& direction, int& hitInfoSize, float maxDistance = std::numeric_limits<float>::infinity() );
		void AddForce(const Balbino::Vector2& force);
		void AddForceAtPosition(const Balbino::Vector2& force, const Balbino::Vector2& position);
		Balbino::Vector2 GetVelocity()const;
		void SetVelocity(Balbino::Vector2& velocity )const;
#ifdef _DEBUG
		virtual void DrawInpector();
#endif // _DEBUG
	private:
		bool m_IsKinematic;
		bool m_FreezRotation;
		bool m_UseGravity;
		float m_AngularDrag;
		float m_Drag;
		float m_Mass;

		const int m_ppm = 8;

		Balbino::RaycastHit m_Hits[128];
		b2Body* m_pBody;

	};
}