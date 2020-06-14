#pragma once
#include "../SceneObject.h"
#include "../Core.h"
#include <vector>
#include <fstream>
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
		~GameObject();

		virtual void Create() override;
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Draw() const override;
#ifdef BALBINO_DEBUG
		virtual void DrawInspector() override;
#endif // BALBINO_DEBUG

		void Destroy();
		bool IsDestroy()const;

		void LoadComponents();
		void SetName( const char* newName );
		const char* GetName()const;
		void OnTriggerEnter(GameObject* pGameObject);
		void OnTriggerExit(GameObject* pGameObject);
		void OnCollisionEnter(GameObject* pGameObject);
		void OnCollisionExit(GameObject* pGameObject);
		template <class T>
		T* GetComponent() const;
		template <class T, class... Args>
		T* AddComponent( Args&&... args );

		void Save( std::ostream& file );
		void Load( std::istream& file );
		void SetActive(bool active);
		bool ActiveInHierarchy()const;

		void SetTag( const std::string& newTag );
		const std::string& GetTag()const;

		GameObject( const GameObject& ) = delete;
		GameObject( GameObject&& ) = delete;
		GameObject& operator= ( const GameObject& ) = delete;
		GameObject& operator= ( const GameObject&& ) = delete;
	private:
		std::vector<Component*> m_Components;
		std::string m_Name;
		std::string m_Tag;
		bool m_IsDestroyed;
		bool m_IsActive;
	};

	template<class T>
	inline T* GameObject::GetComponent() const
	{
		for( Component* comp : m_Components )
		{
			T* component = dynamic_cast< T* >( comp );
			if( component )
			{
				return component;
			}
		}
		return nullptr;
	}

	template<class T, class... Args>
	inline T* GameObject::AddComponent( Args&&... args )
	{
		T* comp{ DBG_NEW T{ this, std::forward<Args>( args )... } };
		m_Components.push_back( comp );
		return comp;
	}
}
#pragma warning(pop)