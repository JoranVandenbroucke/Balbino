#pragma once
#include "Component.h" 
#include "../gl3w/GL/gl3w.h"
#include "../Renderer/Renderer.h"
#include "../Core.h" 
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
		int m_ColorUniformLocation;
		int m_TextureUniformLocation;
		int m_ModelMatricLocation;
		bool m_NeedsUpdate;
		std::string m_Text;
		std::weak_ptr<Font> m_Font;
#ifdef _DEBUG
	public:
		virtual void DrawInpector() override;
	private:
		char* m_InputField{ nullptr };
#endif
	};
}
#pragma warning(pop)