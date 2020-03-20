#pragma once
#include "SceneObject.h"
#include "Text.h"

namespace Balbino
{
	
	class IntroText: public SceneObject
	{
	public:
		IntroText( std::shared_ptr<Font> font );

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw()const override;

	private:
		Text m_Text;
	};
}