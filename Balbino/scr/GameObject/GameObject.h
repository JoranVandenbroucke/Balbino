#pragma once
#include "../SceneObject.h"
#include "../Core.h"
#include <vector>
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
		~GameObject() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
		virtual void Destroy() override;

		void LoadComponents();

		template <class T>
		std::shared_ptr<T> GetComponent();
		template <class T, class... Args>
		std::shared_ptr<T> AddComponent( Args&&... args );
		template< class T>
		std::shared_ptr<T> AddChild();
		void RemoveChild( std::shared_ptr<GameObject> child );
		GameObject( const GameObject& ) = delete;
		GameObject( GameObject&& ) = delete;
		GameObject& operator= ( const GameObject& ) = delete;
		GameObject& operator= ( const GameObject&& ) = delete;
	private:
		std::vector<std::shared_ptr<Component>> m_Components;
		std::weak_ptr<GameObject> m_Parent;
		std::vector<std::shared_ptr<GameObject>> m_Childeren;
	};

	template<class T>
	inline std::shared_ptr<T> GameObject::GetComponent()
	{
		for( std::weak_ptr<Component> comp : m_Components )
		{
			std::shared_ptr<T> component = std::dynamic_pointer_cast< T >( comp.lock() );
			if( component )
			{
				return component;
			}
		}
		return nullptr;
	}

	template<class T, class... Args>
	inline std::shared_ptr<T> GameObject::AddComponent( Args&&... args )
	{
		const std::shared_ptr<GameObject> thisPtr{ weak_from_this() };
		std::shared_ptr<T> comp{ std::make_shared<T>( thisPtr, std::forward<Args>( args )... ) };
		m_Components.push_back( comp );
		return comp;
	}

	template<class T>
	inline std::shared_ptr<T> GameObject::AddChild()
	{
		std::shared_ptr<T> comp{ std::make_shared<T>() };
		m_Childeren.push_back( comp );
		return comp;
	}
}
#pragma warning(pop)