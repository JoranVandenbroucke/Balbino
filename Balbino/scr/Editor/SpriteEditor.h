#pragma once
#include "../Singleton.h"
#include "File.h"
namespace Balbino
{
	class SpriteEditor :public Singleton<SpriteEditor>
	{
	public:
		void Draw();
	private:
		friend class Singleton<SpriteEditor>;
		SpriteEditor() = default;
		file m_Image;
		int m_CountX;
		int m_CountY;
		int m_Widht;
		int m_Height;
		int m_OffsetX;
		int m_OffsetY;
		int m_PaddingX;
		int m_PaddingY;
		float m_PivotX;
		float m_PivotY;
	};
}
