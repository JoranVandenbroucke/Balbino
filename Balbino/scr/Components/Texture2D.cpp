#include "BalbinoPCH.h"
#include "Texture2D.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../ResourceManager.h"
#include <SDL.h>

#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
Balbino::Texture2D::Texture2D( const std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Texture{}
	, m_File{ "" }
	, m_VertexBuff{ (void*) m_Vert, 4 }
	, m_IndexBuff{ (void*) m_Index, 6, sizeof( m_Index[0] ) }
	, m_Shader{ "scr/Shaders/Basic.vs", "scr/Shaders/Basic.fs" }
	, m_Color{ 255,255,255 }
{
	m_VertexBuff.Unbind();

	m_ColorUniformLocation = glGetUniformLocation( m_Shader.GetID(), "u_Color" );
	m_TextureUniformLocation = glGetUniformLocation( m_Shader.GetID(), "u_Texture" );
	m_ModelMatricLocation = glGetUniformLocation( m_Shader.GetID(), "u_Model" );
	if( !( m_ColorUniformLocation != -1 ) )
	{
		glUniform4f( m_ColorUniformLocation, 1.f, 0.f, 1.f, 1.f );
	}

	if( !( m_TextureUniformLocation != -1 ) )
	{
		glUniform1i( m_TextureUniformLocation, 0 );
	}
}

void Balbino::Texture2D::DrawInpector()
{
	if( m_InputField == nullptr )
	{
		m_InputField = const_cast<char*>( m_File.c_str() );
	}
	ImGui::BeginChild( "Texture2D Component", ImVec2{ 420, 64 }, true );
	ImGui::Text( "Texture" );
	ImGui::InputText( "Image Path", m_InputField, 512, ImGuiInputTextFlags_CharsNoBlank ); ImGui::SameLine();
	if( ImGui::Button( "confurm" ) && m_InputField != m_File.c_str() )
	{
		SetTexture( m_InputField );
	}
	ImGui::EndChild();
}
#else
Balbino::Texture2D::Texture2D( const std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Texture{}
	, m_File{ "" }
	, m_VertexBuff{ (void*) m_Vert, 4 }
	, m_IndexBuff{ (void*) m_Index, 6, sizeof( m_Index[0] ) }
	, m_Shader{ "scr/Shaders/Basic.vs", "scr/Shaders/Basic.fs" }
	, m_Color{ 255,255,255 }
{
	m_VertexBuff.Unbind();
	m_Shader.Bind();

	m_ColorUniformLocation = glGetUniformLocation( m_Shader.GetID(), "u_Color" );
	m_TextureUniformLocation = glGetUniformLocation( m_Shader.GetID(), "u_Texture" );
	if( !( m_ColorUniformLocation != -1 ) )
	{
		glUniform4f( m_ColorUniformLocation, 1.f, 0.f, 1.f, 1.f );
	}

	if( !( m_TextureUniformLocation != -1 ) )
	{
		glUniform1i( m_TextureUniformLocation, 0 );
	}
}
#endif

Balbino::Texture2D::~Texture2D()
{
	glDeleteTextures( 1, &m_Texture );
}

void Balbino::Texture2D::SetTexture( const std::string path )
{
	if( m_Texture )
	{
		glDeleteTextures( 1, &m_Texture );
	}
	m_Texture = ResourceManager::LoadTexture( path, m_Vert );
	m_File = path;
	m_VertexBuff.Update( (void*) m_Vert, 4 );
}

void Balbino::Texture2D::SetTexture( GLuint newTexture )
{
	if( m_Texture )
	{
		glDeleteTextures( 1, &m_Texture );
	}
	m_Texture = newTexture;
}

void Balbino::Texture2D::SetColor( const Color& color )
{
	m_Color = color;
}

void Balbino::Texture2D::Create()
{
	this->Component::Create();
}

void Balbino::Texture2D::Update()
{
}

void Balbino::Texture2D::Draw() const
{
	m_Shader.Bind();
	glUniformMatrix4fv( m_ModelMatricLocation, 1, GL_TRUE, &m_Transform.lock()->myModelMatrix[0][0]);
	glUniform4f( m_ColorUniformLocation, m_Color.r / 255.f, m_Color.g / 255.f, m_Color.b / 255.f, m_Color.a / 255.f );
	m_VertexBuff.Bind();
	m_IndexBuff.Bind();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	m_IndexBuff.Unbind();
	m_VertexBuff.Unbind();
	m_Shader.Unbind();
}