#pragma once
#include "SceneObject.h"
#include <vector>
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

namespace Balbino
{
	class Component;

	class BALBINO_API GameObject: public SceneObject, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject();

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		void LoadComponents();

		template <class T>
		std::shared_ptr<T> GetComponent();
		template <class T>
		std::shared_ptr<T> AddComponent();

		GameObject( const GameObject& ) = delete;
		GameObject( GameObject&& ) = delete;
		GameObject& operator= ( const GameObject& ) = delete;
		GameObject& operator= ( const GameObject&& ) = delete;
	private:
		std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<class T>
	inline std::shared_ptr<T> GameObject::GetComponent()
	{
		for( std::shared_ptr<Component> comp : m_Components )
		{
			std::shared_ptr<T> component = std::dynamic_pointer_cast< T >( comp );
			if( component )
			{
				return component;
			}
		}
		return nullptr;
	}

	template<class T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		const std::weak_ptr<GameObject> thisPtr{ weak_from_this() };
		std::shared_ptr<T> comp{ std::make_shared<T>( thisPtr ) };
		m_Components.push_back( comp );
		return comp;
	}
}
#pragma warning(pop)