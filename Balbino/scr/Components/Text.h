#pragma once
#include "Component.h" 
//#include "../gl3w/GL/gl3w.h"
#include "../Renderer/Renderer.h"

namespace Balbino
{
	class GameObject;
	class Font;
	class Texture2D;
	class Text final: public Component
	{
	public:
		explicit Text( const GameObject* const origin );
		virtual ~Text() = default;

		Text( const Text& other ) = delete;
		Text( Text&& other ) = delete;
		Text& operator=( const Text& other ) = delete;
		Text& operator=( Text&& other ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;

		void SetText( const std::string& text );
		void SetColor( const Color& newColor );
		void SetFont( const std::string path, int size );

	private:
		GLuint m_Texture;
		vertex m_Vert[4]
		{
			{-1.0f,-1.0f,0.0f, 0.f,0.f, 1.f,0.f,0.f,1.f},
			{-1.0f, 1.0f,0.0f, 0.f,1.f, 0.f,0.f,1.f,1.f},
			{ 1.0f,-1.0f,0.0f, 1.f,0.f, 0.f,1.f,0.f,1.f},
			{ 1.0f, 1.0f,0.0f, 1.f,1.f, 1.f,1.f,1.f,1.f}
		};
		const Uint32 m_Index[6]
		{
			0,1,2,
			1,2,3
		};
		VertexBuffer m_VertexBuff;
		IndexBuffer m_IndexBuff;
		Shader m_Shader;
		Color m_Color;
		bool m_NeedsUpdate;
		int m_ColorUniformLocation;
		int m_TextureUniformLocation;
		int m_ModelMatricLocation;
		int m_FontSize;
		std::string m_Text;
		std::string m_FontPath;
		Font* m_Font;

#ifdef _DEBUG
	public:
		virtual void DrawInpector() override;
	private:
		char* m_InputField{ nullptr };
#endif
	};
}