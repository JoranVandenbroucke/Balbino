#pragma once
#include "../Singleton.h"
#include "../Struct.h"
#include "../Components/Camera.h"
#include <map>

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

		SDL_Renderer* GetSDLRenderer() const
		{
			return m_Renderer;
		}
	private:
		SDL_Renderer* m_Renderer{};
	};

	struct VertexBuffer
	{
	public:
		VertexBuffer( void* data, uint32_t numVertices );
		virtual ~VertexBuffer();

		void Bind()const;
		void Unbind()const;
		void Update( void* data, uint32_t numVertices ) const;
	private:
		GLuint m_BufferId;
		GLuint m_VAO;
	};

	struct IndexBuffer
	{
	public:
		IndexBuffer( void* data, uint32_t numVertices, uint8_t elementSize );
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
		static void SetCamera( Balbino::Camera& camera );
	private:
		GLuint Compile( const std::string& sourceCode, GLenum type );
		std::string Parse( const char* fileName );
		GLuint CreateShader( const char* vertexShader, const char* fragmentShader );

		GLuint m_ShaderId;
		static int m_Users;
		static std::map<const char*, GLuint> m_Shaders;
		static std::map<std::pair<GLuint, GLuint>, GLuint> m_Programs;
	};
}
