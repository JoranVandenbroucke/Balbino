#pragma once
#include "Collider2D.h"
#include <box2d/box2d.h>

namespace Balbino
{
	class GameObject;
	//https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md55
	class BoxCollider : public Collider2D
	{
	public:
		explicit BoxCollider( const Balbino::GameObject* const origine );
		virtual ~BoxCollider() = default;

		BoxCollider( const BoxCollider& ) = delete;
		BoxCollider( BoxCollider&& ) = delete;
		BoxCollider& operator=( const BoxCollider& ) = delete;
		BoxCollider& operator=( BoxCollider&& ) = delete;

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

		virtual bool RayCast( const Balbino::Ray& ray, Balbino::RaycastHit& hitInfo, float size ) const override;
	private:
		b2PolygonShape m_Collider;

		Balbino::Vector3 m_Center;
		Balbino::Vector3 m_Size;
	};
}
