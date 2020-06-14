#pragma once
#include "../Singleton.h"
#include "File.h"
namespace Balbino
{
	class SpriteEditor :public Singleton<SpriteEditor>
	{
#ifdef BALBINO_DEBUG
	public:
		void Draw();
	private:
		friend class Singleton<SpriteEditor>;
		SpriteEditor();
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

		std::vector<Vector2> m_SpriteUVs;
		const char* m_pItems[13]{ "Auto", "Grid By Cell Size", "Grid By Cell Count",
						"Center", "Top Left", "Top", "Top Right", "Left", "Right", "Bottom Left", "Bottom", "Bottom Right", "Custom" };
		const char* m_CurrentItem{ m_pItems[0] };
		const char* m_CurrentPivot{ m_pItems[3] };
#endif // DEBUG

	};
}
