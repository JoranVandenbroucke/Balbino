#pragma once
#include "Transform.h"
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)

namespace Balbino
{
	class Font;
	class Texture2D;
	class BALBINO_API Text final
	{
	public:
		void Draw() const;

		void SetText(const std::string& text);
		void SetColor(const Color& newColor );
		void SetPosition(float x, float y);

		explicit Text(const std::string& text, const std::shared_ptr<Font>& font);
		virtual ~Text() = default;
		Text(const Text& other) = delete;
		Text(Text&& other) = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) = delete;
	private:
		void Create();
		void Update();
		Color m_Color;
		std::string m_Text;
		Transform m_Transform;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_Texture;
	};
}
#pragma warning(pop)