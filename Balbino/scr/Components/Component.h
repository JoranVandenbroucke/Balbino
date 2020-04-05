#pragma once

namespace Balbino
{
	class GameObject;
	class Transform;

	class Component
	{
	public:
		Component( const std::weak_ptr<GameObject> origin );
		virtual ~Component()=default;

		virtual void Create();
		virtual void Update() = 0;
		virtual void Draw() const = 0;

		template <class T>
		std::shared_ptr<T> GetComponent();
		template <class T, class... Args>
		std::shared_ptr<T> AddComponent(Args&&... args);

		Component( const Component& ) = delete;
		Component( Component&& ) = delete;
		Component& operator= ( const Component& ) = delete;
		Component& operator= ( const Component&& ) = delete;
	protected:
		const std::weak_ptr<GameObject> m_Origin;
		std::weak_ptr<Transform> m_Transform;
	};

	template<class T>
	inline std::shared_ptr<T> Component::GetComponent()
	{
		return m_Origin.lock()->template GetComponent<T>();
	}

	template<class T, class... Args>
	inline std::shared_ptr<T> Component::AddComponent(Args&&... args)
	{
		return m_Origin.lock()->template AddComponent<T>(std::forward<Args>(args)...);
	}
}