#pragma once
#include <filesystem>
#include "../GameObject/GameObject.h"

namespace Balbino
{
	class GameObject;
	class Transform;

	class Component
	{
	public:
		Component( const GameObject* const origin );
		virtual ~Component() = default;

		Component( const Component& ) = delete;
		Component( Component&& ) = delete;
		Component& operator= ( const Component& ) = delete;
		Component& operator= ( const Component&& ) = delete;

		virtual void Create();
		virtual void FixedUpdate()
		{
		};
		virtual void Update() = 0;
		virtual void LateUpdate()
		{
		};
		virtual void Draw() const = 0;

		virtual void Save( std::ostream& file ) = 0;
		virtual void Load( std::istream& file ) = 0;
		virtual GameObject* const GetGameObject() const;
#ifdef _DEBUG
		virtual void DrawInpector() = 0;
#endif // _DEBUG

		template <class T>
		T* GetComponent() const;

		template <class T, class... Args>
		T* AddComponent( Args&&... args );

	protected:
		Balbino::GameObject* const m_pOrigin;
		Balbino::Transform* m_pTransform;
		bool m_Created;
	};


	template<class T>
	inline T* Component::GetComponent() const
	{
		return m_pOrigin->template GetComponent<T>();
	}

	template<class T, class... Args>
	inline T* Component::AddComponent( Args&&... args )
	{
		return m_pOrigin->template AddComponent<T>( std::forward<Args>( args )... );
	}
}