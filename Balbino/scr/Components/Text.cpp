#include "BalbinoPCH.h"
#include "Text.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../ResourceManager.h"
#include "../Font.h"
#include "../BinaryReaderWrider.h"
#include "../Editor/File.h"

#include <SDL.h>
#include <SDL_ttf.h>

Balbino::Text::Text( const std::weak_ptr<GameObject> origin )
	: Component{ origin }
	, m_Text{ "Text" }
	, m_Font{}
	, m_Color{ 255, 255, 255, 255 }
	, m_NeedsUpdate{ true }
	, m_VertexBuff{ (void*) m_Vert, 4 }
	, m_IndexBuff{ (void*) m_Index, 6, sizeof( m_Index[0] ) }
	, m_Shader{ "Shaders/Font.vs", "Shaders/Font.fs" }
	, m_FontPath{ "Lingua.otf" }
	, m_FontSize{ 12 }
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

void Balbino::Text::Create()
{
	m_Transform = GetComponent<Transform>();
	SetFont( m_FontPath, m_FontSize );
}

void Balbino::Text::Update()
{
	if( m_NeedsUpdate )
	{
		if( m_Texture )
		{
			glDeleteTextures( 1, &m_Texture );
		}
		m_Texture = ResourceManager::LoadTexture( *m_Font.lock().get(), m_Text, m_Vert );
		m_VertexBuff.Update( (void*) m_Vert, 4 );
		m_NeedsUpdate = false;
	}
}

void Balbino::Text::Draw() const
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

void Balbino::Text::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::Text ) );
	BinaryReadWrite::Write( file, m_Text );
	BinaryReadWrite::Write( file, m_Color );
	BinaryReadWrite::Write( file, m_FontPath );
	BinaryReadWrite::Write( file, m_FontSize );
}

void Balbino::Text::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_Text );
	BinaryReadWrite::Read( file, m_Color );
	BinaryReadWrite::Read( file, m_FontPath );
	BinaryReadWrite::Read( file, m_FontSize );
	SetFont( m_FontPath, m_FontSize );
}

#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
#include <algorithm>
void Balbino::Text::DrawInpector()
{
	char text[512]{ "" };
	char font[512]{ "" };
	int size = ( std::min )( int( m_Text.size() ), 512 );
	for( int i = 0; i < size; i++ )
	{
		text[i] = m_Text[i];
	}
	size = ( std::min )( int( m_FontPath.size() ), 512 );
	for( int i = 0; i < size; i++ )
	{
		font[i] = m_FontPath[i];
	}
	float color[3]{ m_Color.r / 255.f, m_Color.g / 255.f, m_Color.b / 255.f };
	ImGui::BeginChild( "Text Component", ImVec2{ 420, 128 }, true );
	ImGui::Text( "Text" );
	ImGui::Separator();
	ImGui::InputText( "Text", text, 512 );
	ImGui::ColorEdit3( "Color", color );
	ImGui::InputText( "Font Path", font, 512 ); ImGui::SameLine();
	if( ImGui::BeginDragDropTarget() )
	{
		if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "asset" ) )
		{
			file droppedFile = *(const file*) payload->Data;
			if( droppedFile.type == fileTypes::font )
			{
				SetFont( std::filesystem::relative( droppedFile.path ).string(), m_FontSize );
			}
		}
		ImGui::EndDragDropTarget();
	}
	if( ImGui::Button( "submit" ) )
	{
		SetFont( font, m_FontSize );
		m_NeedsUpdate = true;
		m_Text = text;
		m_Color.r = char( color[0] * 255 );
		m_Color.g = char( color[1] * 255 );
		m_Color.b = char( color[2] * 255 );
	}
	ImGui::InputInt( "size", &m_FontSize );
	ImGui::EndChild();

	if( strcmp( m_Text.c_str(), text ) || m_Color.r != unsigned char( color[0] * 255 ) || m_Color.g != unsigned char( color[1] * 255 ) || m_Color.b != unsigned char( color[2] * 255 ) )
	{
		m_NeedsUpdate = true;
		m_Text = text;
		m_Color.r = char( color[0] * 255 );
		m_Color.g = char( color[1] * 255 );
		m_Color.b = char( color[2] * 255 );
	}

}
#endif

// This implementation uses the "dirty flag" pattern
void Balbino::Text::SetText( const std::string& text )
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void Balbino::Text::SetColor( const Color& newColor )
{
	m_Color = newColor;
	m_NeedsUpdate = true;
}

void Balbino::Text::SetFont( const std::string path, int size )
{
	m_FontPath = path;
	m_FontSize = size;
	m_Font = ResourceManager::LoadFont( path, size );
	m_NeedsUpdate = true;
}
