#pragma once
#include "Component.h"

namespace Balbino
{
	class GameObject;
	class Collider2D : public Component, public std::enable_shared_from_this<Collider2D>
	{
	public:
		explicit Collider2D( const GameObject* const origine );


		Collider2D( const Collider2D& ) = delete;
		Collider2D( Collider2D&& ) = delete;
		Collider2D& operator=( const Collider2D& ) = delete;
		Collider2D& operator=( Collider2D&& ) = delete;

		virtual ~Collider2D() = default;

		virtual bool RayCast( const Balbino::Ray& ray, Balbino::RaycastHit& hitInfo, float size ) const = 0;
	private:

	};
}