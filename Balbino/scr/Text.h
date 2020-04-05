#pragma once
#include "Transform.h"
#include "Component.h" 
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

namespace Balbino
{
	class GameObject;
	class Font;
	class Texture2D;
	class BALBINO_API Text final: public Component
	{
	public:
		explicit Text( const std::weak_ptr<GameObject> origin );
		virtual ~Text() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		void SetText( const std::string& text );
		void SetColor( const Color& newColor );
		void SetPosition( float x, float y );
		void SetFont( const std::string path, int size );

		Text( const Text& other ) = delete;
		Text( Text&& other ) = delete;
		Text& operator=( const Text& other ) = delete;
		Text& operator=( Text&& other ) = delete;
	private:
		bool m_NeedsUpdate;
		Color m_Color;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_Texture;
	};
}
#pragma warning(pop)