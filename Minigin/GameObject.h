#pragma once
#include "Transform.h"
#include "SceneObject.h"

namespace Balbino
{
	class Texture2D;
	class GameObject : public SceneObject
	{
	public:
		void Create() override;
		void Update() override;
		void Draw() const override;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture{};
	};
}
