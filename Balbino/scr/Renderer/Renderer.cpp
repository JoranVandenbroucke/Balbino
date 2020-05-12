#include "BalbinoPCH.h"
#include "Renderer.h"
//#include <SDL.h>
#include "../SceneManager.h"
#include "../Components/Texture2D.h"
#include "../Application.h"

#include <fstream> 

#include "../Scene.h"

#include <iostream>
#include <sstream>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

void Balbino::Renderer::Init( SDL_Window* window )
{
	m_Renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( m_Renderer == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError() );
	}
}

void Balbino::Renderer::Destroy()
{
	if( m_Renderer != nullptr )
	{
		SDL_DestroyRenderer( m_Renderer );
		m_Renderer = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Balbino::VertexBuffer::VertexBuffer( void* data, uint32_t numVertices )
{
	glGenVertexArrays( 1, &m_VAO );
	glBindVertexArray( m_VAO );

	glGenBuffers( 1, &m_BufferId );
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferId );
	glBufferData( GL_ARRAY_BUFFER, numVertices * sizeof( vertex ), data, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( vertex ), (void*) offsetof( struct vertex, x ) );

	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( vertex ), (void*) offsetof( struct vertex, u ) );

	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( vertex ), (void*) offsetof( struct vertex, r ) );

	glBindVertexArray( 0 );
}

Balbino::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers( 1, &m_BufferId );
}

void Balbino::VertexBuffer::Bind()const
{
	glBindVertexArray( m_VAO );
}

void Balbino::VertexBuffer::Unbind()const
{
	glBindVertexArray( 0 );
}

void Balbino::VertexBuffer::Update( void* data, uint32_t numVertices )const
{
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferId );
	glBufferData( GL_ARRAY_BUFFER, numVertices * sizeof( vertex ), data, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Balbino::IndexBuffer::IndexBuffer( void* data, uint32_t numVertices, uint8_t elementSize )
{
	glGenBuffers( 1, &m_BufferId );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_BufferId );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, numVertices * elementSize, data, GL_STATIC_DRAW );
}

Balbino::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers( 1, &m_BufferId );
}

void Balbino::IndexBuffer::Bind() const
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_BufferId );
}

void Balbino::IndexBuffer::Unbind() const
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
int Balbino::Shader::m_Users{ 0 };
std::map<const char*, GLuint> Balbino::Shader::m_Shaders{};
std::map<std::pair<GLuint, GLuint>, GLuint> Balbino::Shader::m_Programs{};

Balbino::Shader::Shader( const char* vertexShader, const char* fragmentShader )
	:m_ShaderId{ CreateShader( vertexShader, fragmentShader ) }
{
	++m_Users;
}

Balbino::Shader::~Shader()
{
	if( --m_Users == 0 )
	{
		m_Shaders.clear();
		for( auto& program : m_Programs )
		{
			glDeleteProgram( program.second );
		}
		m_Programs.clear();
	}
}

void Balbino::Shader::Bind() const
{
	glUseProgram( m_ShaderId );
}

void Balbino::Shader::Unbind() const
{
	glUseProgram( 0 );
}

void Balbino::Shader::SetCamera( Balbino::Camera& camera )
{
	for( auto shader : m_Programs )
	{
		glUseProgram( shader.second );
		GLint location = glGetUniformLocation( shader.second, "u_ViewProjection"  );
		glUniformMatrix4fv( location, 1, GL_TRUE, glm::value_ptr( camera.GetViewProjectionMatrix() ) );
	}
}

GLuint Balbino::Shader::Compile( const std::string& sourceCode, GLenum type )
{
	GLuint id{ glCreateShader( type ) };
	const char* scr{ sourceCode.c_str() };
	glShaderSource( id, 1, &scr, 0 );
	glCompileShader( id );

	int result{};
	glGetShaderiv( id, GL_COMPILE_STATUS, &result );
	if( result != GL_TRUE )
	{
		int length{};
		glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
		char* message = new char[length];
		glGetShaderInfoLog( id, length, &length, message );
		std::string ErrorSting{ "Shader compilation error: " };
		ErrorSting += message;
		delete[] message;
		throw std::runtime_error{ ErrorSting.c_str() };
	}

	return id;
}

std::string Balbino::Shader::Parse( const char* fileName )
{
	try
	{
		std::ifstream file{};
		file.exceptions( std::ifstream::failbit );
		file.open( fileName, std::ios::binary | std::ios::in );
		if( !file.is_open() )
		{
			throw std::runtime_error{ "could not open shader" };
		}
		std::string content( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );
		file.close();
		return content;
	}
	catch( const std::ios_base::failure& f )
	{
		std::cerr << "Caught error: " << f.what() << f.code().message() << '\n';
		std::stringstream error{};
		error << GetLastError();
		std::cerr << error.str();
	}
	return 0;
}

GLuint Balbino::Shader::CreateShader( const char* vertexShader, const char* fragmentShader )
{
	GLuint program{ glCreateProgram() };
	GLuint vs{};
	GLuint fs{};

	if( m_Shaders.find( vertexShader ) == m_Shaders.end() )
	{
		std::string vertexShaderSource = Parse( vertexShader );
		vs = Compile( vertexShaderSource, GL_VERTEX_SHADER );
		m_Shaders[vertexShader] = vs;
	}
	else
	{
		vs = m_Shaders[vertexShader];
	}

	if( m_Shaders.find( fragmentShader ) == m_Shaders.end() )
	{
		std::string fragmentShaderSource = Parse( fragmentShader );
		fs = Compile( fragmentShaderSource, GL_FRAGMENT_SHADER );
		m_Shaders[fragmentShader] = fs;
	}
	else
	{
		fs = m_Shaders[fragmentShader];
	};

	if( m_Programs.find(std::make_pair(vs,fs)) != m_Programs.end() )
	{
		return m_Programs[std::make_pair( vs, fs )];
	}

	glAttachShader( program, vs );
	glAttachShader( program, fs );
	glLinkProgram( program );

#ifndef _DEBUG
	glDetachShader( program, vs );
	glDetachShader( program, fs );

	glDeleteShader( vs );
	glDeleteShader( fs );
#endif

	return m_Programs[std::make_pair( vs, fs )] = program;
}
