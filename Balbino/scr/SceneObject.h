#pragma once
#include "Core.h"
#include "Transform.h"

namespace Balbino
{
	class SceneObject
	{
	public:
		virtual void Create() = 0;
		virtual void Update() = 0;
		virtual void Draw() const = 0;
		
		const glm::vec3& GetPosition()const
		{
			return m_Transform.GetPosition();
		}

		SceneObject() = default;
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;
	protected:
		Transform m_Transform;
	};
}
