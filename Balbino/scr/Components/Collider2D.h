#pragma once
#include "Component.h"

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
	private:

	};
}