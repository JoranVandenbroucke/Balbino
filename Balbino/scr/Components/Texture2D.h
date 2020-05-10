#pragma once
#include "Component.h"
#include "../gl3w/GL/gl3w.h"
#include "../Renderer/Renderer.h"

struct SDL_Texture;
namespace Balbino
{
	class GameObject;

	class Texture2D : public Component
	{
	public:
		explicit Texture2D(const GameObject* const origine );

		virtual ~Texture2D();

		void SetTexture( const std::string path );
		void SetTexture( GLuint newTexture );
		void SetColor( const Color& color );

		virtual void Update() override;
		virtual void Draw() const override;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	protected:
		GLuint m_Texture;

		Balbino::vertex m_Vert[4]
		{
			{-1.0f,-1.0f,0.0f, 0.f,0.f, 1.f,0.f,0.f,1.f},
			{-1.0f, 1.0f,0.0f, 0.f,1.f, 0.f,0.f,1.f,1.f},
			{ 1.0f,-1.0f,0.0f, 1.f,0.f, 0.f,1.f,0.f,1.f},
			{ 1.0f, 1.0f,0.0f, 1.f,1.f, 1.f,1.f,1.f,1.f}
		};
		const UINT32 m_Index[6]
		{
			0,1,2,
			1,2,3
		};
		VertexBuffer m_VertexBuff;
		IndexBuffer m_IndexBuff;
		Shader m_Shader;
		Color m_Color;
		Vector2 m_Dimentions;
		std::string m_File;
		int m_ColorUniformLocation;
		int m_TextureUniformLocation;
		int m_ModelMatricLocation;
	};
}