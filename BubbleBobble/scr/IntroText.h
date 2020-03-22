#pragma once

namespace BubbleBobble
{
	
	class IntroText: public Balbino::SceneObject
	{
	public:
		IntroText( std::shared_ptr<Balbino::Font> font );

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw()const override;

	private:
		Balbino::Text m_Text;
	};
}