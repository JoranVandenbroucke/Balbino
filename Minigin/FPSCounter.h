#pragma once
#include "Transform.h"
#include "SceneObject.h"
namespace Balbino
{
	class TextObject;
	class FPSCounter: public SceneObject
	{
	public:
		void Create() override;
		void Update() override;
		void Draw() const override;
	private:
		Transform m_Transform;
		std::shared_ptr<TextObject> m_Text{};
	};
}
