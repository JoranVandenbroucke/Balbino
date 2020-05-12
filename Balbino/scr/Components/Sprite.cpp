#include "BalbinoPCH.h"
#include "Sprite.h"
#include "../imgui-1.75/imgui.h"
#include "../Editor/File.h"
#include "../BinaryReaderWrider.h"

#include <filesystem>
#include <fstream>

Balbino::Sprite::Sprite( const GameObject* const origine )
	: Texture2D{ origine }
	, m_SpriteIndex{}
{
}

Balbino::Sprite::~Sprite()
{
	m_UV.clear();
}

void Balbino::Sprite::SetSpriteIndex( int index )
{
	if( index < 0 ) index = 0;

	index = ( std::min )( int( m_UV.size() / 2 - 1 ), index );
	m_Vert[0].u = m_UV[index * 2].x;
	m_Vert[0].v = m_UV[index * 2].y;

	m_Vert[1].u = m_UV[index * 2].x;
	m_Vert[1].v = m_UV[index * 2 + 1].y;

	m_Vert[2].u = m_UV[index * 2 + 1].x;
	m_Vert[2].v = m_UV[index * 2].y;

	m_Vert[3].u = m_UV[index * 2 + 1].x;
	m_Vert[3].v = m_UV[index * 2 + 1].y;

	m_Vert[0].x = -( m_UV[index * 2 + 1].x - m_UV[index * 2].x ) / 2.f * m_Dimentions.x;
	m_Vert[0].y = -( m_UV[index * 2 + 1].y - m_UV[index * 2].y ) / 2.f * m_Dimentions.y;
	m_Vert[1].x = m_Vert[0].x;
	m_Vert[1].y = -m_Vert[0].y;
	m_Vert[2].x = -m_Vert[0].x;
	m_Vert[2].y = m_Vert[0].y;
	m_Vert[3].x = -m_Vert[0].x;
	m_Vert[3].y = -m_Vert[0].y;
	m_SpriteIndex = index;
	m_VertexBuff.Update( (void*) m_Vert, 4 );
}

void Balbino::Sprite::LoadUV()
{
	std::ifstream uvFile{ "../Data/" + m_File + ".spr", std::ios::in | std::ios::binary };
	if( uvFile.is_open() )
	{
		int size{};
		BinaryReadWrite::Read( uvFile, size );
		m_UV.resize( size );
		for( int i = 0; i < size; i++ )
		{
			BinaryReadWrite::Read( uvFile, m_UV[i] );
		}
	}
	else
	{
		m_UV = { {0,0}, {1,1} };
	}
	uvFile.close();
}

const Balbino::Vector2& Balbino::Sprite::GetPivot() const
{
	return m_Pivot;
}

const std::vector<Balbino::Vector2>& Balbino::Sprite::uv() const
{
	return m_UV;
}

void Balbino::Sprite::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, m_SpriteIndex );
	BinaryReadWrite::Write( file, m_Color );
	BinaryReadWrite::Write( file, m_File );
}

void Balbino::Sprite::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_SpriteIndex );
	BinaryReadWrite::Read( file, m_Color );
	BinaryReadWrite::Read( file, m_File );
	LoadUV();
	SetTexture( m_File );
	SetSpriteIndex( m_SpriteIndex );
}

#ifdef _DEBUG
void Balbino::Sprite::DrawInpector()
{
	//set variables
	char inputField[256]{ "" };
	int size = ( std::min )( int( m_File.size() ), 256 );
	for( int i = 0; i < size; i++ )
	{
		inputField[i] = m_File[i];
	}

	float color[4]{ m_Color.r / 255.f, m_Color.g / 255.f, m_Color.b / 255.f, m_Color.a / 255.f };
	int spriteIndex{ m_SpriteIndex };
	bool confirm{ false };
	bool dragAndDrop{ false };

	//set ImGui
	ImGui::BeginChild( "Sprite Component", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Texture" );
	ImGui::InputText( "Image Path", inputField, 256 ); ImGui::SameLine();
	dragAndDrop = ImGui::BeginDragDropTarget();
	confirm = ImGui::Button( "confurm" );
	ImGui::InputInt( "Sprite Nr", &spriteIndex );
	ImGui::ColorEdit4( "Color", color );

	//Use the Outcome
	if( dragAndDrop )
	{
		if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "asset" ) )
		{
			file droppedFile = *(const file*) payload->Data;
			if( droppedFile.type == fileTypes::image )
			{
				SetTexture( droppedFile.path.string() );
				int fileSize = ( std::min )( int( m_File.size() ), 256 );
				for( int i = 0; i < fileSize; i++ )
				{
					inputField[i] = m_File[i];
				}
				LoadUV();
				SetSpriteIndex( m_SpriteIndex );
			}
		}
		ImGui::EndDragDropTarget();
	}
	if( confirm && ( strlen(inputField) != 0 || inputField != m_File ) )
	{
		SetTexture( inputField );
		LoadUV();
		SetSpriteIndex( m_SpriteIndex );
	}

	if( spriteIndex != m_SpriteIndex )
	{
		SetSpriteIndex( spriteIndex );
	}

	m_Color.r = (unsigned char) ( color[0] * 255 );
	m_Color.g = (unsigned char) ( color[1] * 255 );
	m_Color.b = (unsigned char) ( color[2] * 255 );
	m_Color.a = (unsigned char) ( color[3] * 255 );

	ImGui::EndChild();
}
#endif // _DEBUG

