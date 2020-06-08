#pragma once
#include "Component.h"
class b2Body;
namespace Balbino
{
	class GameObject;
	class Collider2D : public Component
	{
	public:
		explicit Collider2D( const GameObject* const origine );
		virtual ~Collider2D() = default;

		Collider2D( const Collider2D& ) = delete;
		Collider2D( Collider2D&& ) = delete;
		Collider2D& operator=( const Collider2D& ) = delete;
		Collider2D& operator=( Collider2D&& ) = delete;

		virtual bool RayCast( const Balbino::Ray& ray, Balbino::RaycastHit& hitInfo, float size ) const = 0;

		// Inherited via Component
		virtual void Update() override = 0;
		virtual void Draw() const override = 0;
		virtual void Save( std::ostream& file ) override = 0;
		virtual void Load( std::istream& file ) override = 0;
#ifdef _DEBUG
		virtual void DrawInpector() override = 0;
#endif // _DEBUG

	protected:
		friend class Rigidbody2D;
		b2Body* m_pBody;
	};
}