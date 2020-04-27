#pragma once
#include "../Singleton.h"
#include "../Struct.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace Balbino
{
	class Texture2D;
	struct VertexBuffer;
	struct IndexBuffer;
	struct Shader;
	class AssetBrouser;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	struct vertex
	{
		float x, y, z;
		float u, v;
		float r, g, b, a;
	};

	class Renderer final: public Singleton<Renderer>
	{
	public:
		void Init( SDL_Window* window );
		void Destroy();

		void RenderTexture( const GLuint& texture, float x, float y ) const;
		void RenderTexture( const GLuint& texture, float x, float y, float width, float height ) const;

		SDL_Renderer* GetSDLRenderer() const
		{
			return m_Renderer;
		}
	private:
		SDL_Renderer* m_Renderer{};
#ifdef _DEBUG
	public:
		void Draw();
	private:
		GLuint m_FrameBufferIndex{};
		GLuint m_Rbo{};
		uint32_t m_RenderedTexture{};
		GLenum m_DrawBuffers[1];
		void Bind() const;
		void Unbind() const;
#else
	public:
		void Draw() const;
#endif // _DEBUG

	};

	struct VertexBuffer
	{
	public:
		VertexBuffer( void* data, Uint32 numVertices );
		virtual ~VertexBuffer();

		void Bind()const;
		void Unbind()const;
		void Update( void* data, Uint32 numVertices ) const;
	private:
		GLuint m_BufferId;
		GLuint m_VAO;
	};

	struct IndexBuffer
	{
	public:
		IndexBuffer( void* data, Uint32 numVertices, Uint8 elementSize );
		virtual ~IndexBuffer();

		void Bind()const;
		void Unbind()const;
	private:
		GLuint m_BufferId;
	};

	struct Shader
	{
	public:
		Shader( const char* vertexShader, const char* fragmentShader );
		virtual ~Shader();

		GLuint GetID()
		{
			return m_ShaderId;
		}

		void Bind()const;
		void Unbind()const;
	private:
		GLuint Compile( const std::string& sourceCode, GLenum type );
		std::string Parse( const char* fileName );
		GLuint CreateShader( const char* vertexShader, const char* fragmentShader );

		GLuint m_ShaderId;

	};
}
