#include "BalbinoPCH.h"
#include "Texture2D.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../ResourceManager.h"
#include "../BinaryReaderWrider.h"
#include "../Editor/File.h"
#include <SDL.h>

#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
Balbino::Texture2D::Texture2D( const std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Texture{}
	, m_File{ "" }
	, m_VertexBuff{ (void*) m_Vert, 4 }
	, m_IndexBuff{ (void*) m_Index, 6, sizeof( m_Index[0] ) }
	, m_Shader{ "Shaders/Basic.vs", "Shaders/Basic.fs" }
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
	float color[4]{ m_Color.r / 255.f, m_Color.g / 255.f, m_Color.b / 255.f, m_Color.a / 255.f };
	ImGui::BeginChild( "Texture2D Component", ImVec2{ 420, 96 }, true );
	ImGui::Text( "Texture" );
	ImGui::InputText( "Image Path", m_InputField, 512 ); ImGui::SameLine();
	if( ImGui::BeginDragDropTarget() )
	{
		if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "asset" ) )
		{
			file droppedFile = *(const file*) payload->Data;
			if( droppedFile.type == fileTypes::image )
			{
				SetTexture( std::filesystem::relative( droppedFile.path ).generic_u8string() );
			}
		}
		ImGui::EndDragDropTarget();
	}
	if( ImGui::Button( "confurm" ) )
	{
		SetTexture( m_InputField );
	}
	ImGui::ColorEdit4( "Color", color );
	m_Color.r = (unsigned char) ( color[0] * 255 );
	m_Color.g = (unsigned char) ( color[1] * 255 );
	m_Color.b = (unsigned char) ( color[2] * 255 );
	m_Color.a = (unsigned char) ( color[3] * 255 );

	ImGui::EndChild();
}
#else
Balbino::Texture2D::Texture2D( const std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Texture{}
	, m_File{ "" }
	, m_VertexBuff{ (void*) m_Vert, 4 }
	, m_IndexBuff{ (void*) m_Index, 6, sizeof( m_Index[0] ) }
	, m_Shader{ "Shaders/Basic.vs", "Shaders/Basic.fs" }
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
	m_Texture = ResourceManager::LoadTexture( path, m_Vert );
	m_File = path;
	m_VertexBuff.Update( (void*) m_Vert, 4 );
}

void Balbino::Texture2D::SetTexture( GLuint newTexture )
{
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
	glUniformMatrix4fv( m_ModelMatricLocation, 1, GL_TRUE, &m_Transform.lock()->TransfomationMatrix[0][0] );
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

void Balbino::Texture2D::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::Texture2D ) );
	BinaryReadWrite::Write( file, m_File );
	BinaryReadWrite::Write( file, m_Color );
}

void Balbino::Texture2D::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_File );
	BinaryReadWrite::Read( file, m_Color );
	SetTexture( m_File );
}
