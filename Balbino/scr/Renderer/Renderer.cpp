#include "BalbinoPCH.h"
#include "Renderer.h"
//#include <SDL.h>
#include "../SceneManager.h"
#include "../Components/Texture2D.h"
#include "../Debug.h"
#include <fstream> 

#include "../imgui-1.75/imgui.h"
#include "../imgui-1.75/imgui_impl_opengl3.h"
#include "../imgui-1.75/imgui_impl_sdl.h"

#include <iostream>
#ifdef _DEBUG
#include "../gl3w/GL/gl3w.h"
#include "../Application.h"
void Balbino::Renderer::Init( SDL_Window* window )
{
	m_Renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( m_Renderer == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError() );
	}

	glGenFramebuffers( 1, &m_FrameBufferIndex );
	glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferIndex );
	glDrawBuffer( GL_COLOR_ATTACHMENT0 );

	/*
	glGenTextures(1, &m_RenderedTexture);
	glBindTexture(GL_TEXTURE_2D, m_RenderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 640, 480, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_RenderedTexture, 0);

	*/

	glGenTextures( 1, &m_RenderedTexture );
	glBindTexture( GL_TEXTURE_2D, m_RenderedTexture );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderedTexture, 0 );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		Debug::LogError( "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void Balbino::Renderer::Draw() const
{
	//start ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame( Application::GetWindow() );
	ImGui::NewFrame();


	glViewport( 0, 0, (int) 1920, (int) 1080 );
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	Bind();
	glViewport( 0, 0, (int) 640, (int) 480 );
	glClearColor( 0.f, 1.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT ); // we're not using the stencil buffer now
	glEnable( GL_DEPTH_TEST );
	SceneManager::Get().Draw();
	Unbind();

	//Draw All Of Im Gui
	//ImGui::ShowDemoWindow();
	Debug::Get().Draw();
	SceneManager::Get().DrawEngine();

	ImGui::Begin( "GameView" );
	ImGui::BeginChild( "game", ImVec2{ 640,480 } );
	//ImGui::Image( (ImTextureID) (intptr_t) m_RenderedTexture, ImVec2{ 640,480 } );
	ImGui::GetWindowDrawList()->AddImage( (void*) (uint64_t) m_RenderedTexture,
		ImVec2{ ImGui::GetCursorScreenPos() },
		ImVec2{ ImGui::GetCursorScreenPos().x + 640, ImGui::GetCursorScreenPos().y + 480 } );
	ImGui::EndChild();
	ImGui::End();

	// Rendering Game
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
	ImGuiIO& io = ImGui::GetIO(); (void) io;

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent( backup_current_window, backup_current_context );
	}
	SDL_GL_SwapWindow( Application::GetWindow() );
}

void Balbino::Renderer::Bind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferIndex );
}

void Balbino::Renderer::Unbind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
void Balbino::Renderer::Destroy()
{
	glDeleteFramebuffers( 1, &m_FrameBufferIndex );
	if( m_Renderer != nullptr )
	{
		SDL_DestroyRenderer( m_Renderer );
		m_Renderer = nullptr;
	}
}
#else
void Balbino::Renderer::Init( SDL_Window* window )
{
	m_Renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( m_Renderer == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError() );
	};
}

void Balbino::Renderer::Draw() const
{
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	glClear( GL_COLOR_BUFFER_BIT );
	SceneManager::Get().Draw();
	SDL_GL_SwapWindow( Application::GetWindow() );
}
void Balbino::Renderer::Destroy()
{
	if( m_Renderer != nullptr )
	{
		SDL_DestroyRenderer( m_Renderer );
		m_Renderer = nullptr;
	}
}
#endif // _DEBUG


void Balbino::Renderer::RenderTexture( const GLuint& texture, const float x, const float y ) const
{
	int w, h;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, texture, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, texture, GL_TEXTURE_HEIGHT, &h );
	RenderTexture( texture, x, y, (const float) w, (const float) h );
}

void Balbino::Renderer::RenderTexture( const GLuint& texture, const float x, const float y, const float width, const float height ) const
{
	(void) texture;
	(void) x;
	(void) y;
	(void) width;
	(void) height;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Balbino::VertexBuffer::VertexBuffer( void* data, Uint32 numVertices )
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

void Balbino::VertexBuffer::Update( void* data, Uint32 numVertices )
{
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferId );
	glBufferData( GL_ARRAY_BUFFER, numVertices * sizeof( vertex ), data, GL_STATIC_DRAW );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Balbino::IndexBuffer::IndexBuffer( void* data, Uint32 numVertices, Uint8 elementSize )
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
Balbino::Shader::Shader( const char* vertexShader, const char* fragmentShader )
	:m_ShaderId{ CreateShader( vertexShader, fragmentShader ) }
{
}

Balbino::Shader::~Shader()
{
	glDeleteProgram( m_ShaderId );
}

void Balbino::Shader::Bind() const
{
	glUseProgram( m_ShaderId );
}

void Balbino::Shader::Unbind() const
{
	glUseProgram( 0 );
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
	std::ifstream file{ fileName };
	if( !file.is_open() )
	{
		throw std::runtime_error{ "could not open shader" };
	}
	std::string content( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );
	file.close();
	return content;
}

GLuint Balbino::Shader::CreateShader( const char* vertexShader, const char* fragmentShader )
{
	std::string vertexShaderSource = Parse( vertexShader );
	std::string fragmentShaderSource = Parse( fragmentShader );

	GLuint program = glCreateProgram();
	GLuint vs = Compile( vertexShaderSource, GL_VERTEX_SHADER );
	GLuint fs = Compile( fragmentShaderSource, GL_FRAGMENT_SHADER );

	glAttachShader( program, vs );
	glAttachShader( program, fs );
	glLinkProgram( program );

#ifndef _DEBUG
	glDetachShader( program, vs );
	glDetachShader( program, fs );

	glDeleteShader( vs );
	glDeleteShader( fs );
#endif

	return program;
}
