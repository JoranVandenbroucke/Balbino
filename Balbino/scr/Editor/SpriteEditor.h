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
		void DrawOptoins( std::vector<Vector2>& sprites );
		void DrawImageSlicer( std::vector<Vector2>& sprites );
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
