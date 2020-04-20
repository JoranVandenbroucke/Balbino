#pragma once
#include "Component.h"
#include "../gl3w/GL/gl3w.h"
#include "../Renderer/Renderer.h"
#include "../Core.h"
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

struct SDL_Texture;
namespace Balbino
{
	class GameObject;

	class BALBINO_API Texture2D final: public Component
	{
	public:
		explicit Texture2D(const std::weak_ptr<GameObject> origine );
		virtual ~Texture2D();

		void SetTexture( const std::string path );
		void SetTexture( GLuint newTexture );
		void SetColor( const Color& color );

		virtual void Create();
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		GLuint m_Texture;
		std::string m_File;
		vertex m_Vert[4]
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
		int m_ColorUniformLocation;
		int m_TextureUniformLocation;
		int m_ModelMatricLocation;
#ifdef _DEBUG
	public:
		virtual void DrawInpector() override;

	private:
		char* m_InputField{nullptr};
#endif
	};
}
#pragma warning(pop)