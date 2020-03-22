#pragma once

namespace BubbleBobble
{
	class Balbino::Text;
	class FPSCounter: public Balbino::SceneObject
	{
	public:
		void Create() override;
		void Update() override;
		void Draw() const override;
	private:
		Balbino::Transform m_Transform;
		std::shared_ptr<Balbino::Text> m_Text{};
	};
}
