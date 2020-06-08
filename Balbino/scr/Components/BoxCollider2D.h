#pragma once
#include "Collider2D.h"
#include <box2d.h>

namespace Balbino
{
	class GameObject;
	//https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md55
	class BoxCollider2D : public Collider2D
	{
	public:
		explicit BoxCollider2D( const Balbino::GameObject* const origine );
		virtual ~BoxCollider2D() = default;

		BoxCollider2D( const BoxCollider2D& ) = delete;
		BoxCollider2D( BoxCollider2D&& ) = delete;
		BoxCollider2D& operator=( const BoxCollider2D& ) = delete;
		BoxCollider2D& operator=( BoxCollider2D&& ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream & file )override;
		virtual void Load( std::istream & file )override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		const Balbino::Vector3& GetCenter() const;
		const Balbino::Vector3& GetSize() const;
		void SetCenter(const Balbino::Vector3& newCenter);
		void SetSize( const Balbino::Vector3& newSize );
		void SetTrigger( bool isTrigger );
		virtual bool RayCast( const Balbino::Ray& ray, Balbino::RaycastHit& hitInfo, float size ) const override;
	private:
		Balbino::Vector3 m_Center;
		Balbino::Vector3 m_Size;
		b2BodyDef m_BodyDef;
		b2Fixture* m_pFixture;
		b2PolygonShape m_Colliser;
	};
}
