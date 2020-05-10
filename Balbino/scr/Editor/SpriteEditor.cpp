#include "BalbinoPCH.h"
#include "SpriteEditor.h"
#include "../ResourceManager.h"
#include "Debug.h"
#include "../BinaryReaderWrider.h"
#include <fstream>
#include <algorithm>

void Balbino::SpriteEditor::Draw()
{

	static std::vector<Vector2> spriteUVs;
	ImGui::Begin( "Sprite Editor" );
	DrawOptoins( spriteUVs );
	ImGui::SameLine();
	DrawImageSlicer( spriteUVs );
	ImGui::End();
}

void Balbino::SpriteEditor::DrawOptoins( std::vector<Vector2>& sprites )
{
	const char* items[13] = { "Auto", "Grid By Cell Size", "Grid By Cell Count",
						"Center", "Top Left", "Top", "Top Right", "Left", "Right", "Bottom Left", "Bottom", "Bottom Right", "Custom" };
	static const char* currentItem = items[0];
	static const char* currentPivot = items[3];

	ImGui::BeginChild( "editor", { 350,-1 }, true );
	if( ImGui::BeginCombo( "Mode", currentItem ) )
	{
		for( int i = 0; i < 3; i++ )
		{
			bool isSelected = ( currentItem == items[i] );
			if( ImGui::Selectable( items[i], isSelected ) )
				currentItem = items[i];
			if( isSelected )
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}

	if( currentItem == items[1] )
	{
		ImGui::Columns( 3 );
		ImGui::Text( "Pixel Size" ); ImGui::NextColumn();
		ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragInt( "##SizeX", &m_Widht ); ImGui::NextColumn();
		ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragInt( "##SizeY", &m_Height ); ImGui::NextColumn();

		ImGui::Text( "Offset" ); ImGui::NextColumn();
		ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragInt( "##OffsetX", &m_OffsetX ); ImGui::NextColumn();
		ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragInt( "##OffsetY", &m_OffsetY ); ImGui::NextColumn();

		ImGui::Text( "Padding" );  ImGui::NextColumn();
		ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragInt( "##PaddingX", &m_PaddingX );  ImGui::NextColumn();
		ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragInt( "##PaddingY", &m_PaddingY ); ImGui::NextColumn();
		ImGui::Columns( 1 );
		( m_Widht <= 0 ) ? m_Widht = 1 : 0;
		( m_Height <= 0 ) ? m_Height = 1 : 0;
		( m_OffsetX < 0 ) ? m_OffsetX = 0 : 0;
		( m_OffsetY < 0 ) ? m_OffsetY = 0 : 0;
		( m_PaddingX < 0 ) ? m_PaddingX = 0 : 0;
		( m_PaddingY < 0 ) ? m_PaddingY = 0 : 0;
	}
	else if( currentItem == items[2] )
	{
		ImGui::Columns( 3 );
		ImGui::Text( "Column & Row" ); ImGui::NextColumn();
		ImGui::Text( "C:" ); ImGui::SameLine(); ImGui::DragInt( "##SizeX", &m_CountX ); ImGui::NextColumn();
		ImGui::Text( "R:" ); ImGui::SameLine(); ImGui::DragInt( "##SizeY", &m_CountY ); ImGui::NextColumn();

		ImGui::Text( "Offset" ); ImGui::NextColumn();
		ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragInt( "##OffsetX", &m_OffsetX ); ImGui::NextColumn();
		ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragInt( "##OffsetY", &m_OffsetY ); ImGui::NextColumn();

		ImGui::Text( "Padding" ); ImGui::NextColumn();
		ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragInt( "##PaddingX", &m_PaddingX ); ImGui::NextColumn();
		ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragInt( "##PaddingY", &m_PaddingY ); ImGui::NextColumn();
		ImGui::Columns( 1 );
		( m_CountX <= 0 ) ? m_CountX = 1 : 0;
		( m_CountY <= 0 ) ? m_CountY = 1 : 0;
		( m_OffsetX < 0 ) ? m_OffsetX = 0 : 0;
		( m_OffsetY < 0 ) ? m_OffsetY = 0 : 0;
		( m_PaddingX < 0 ) ? m_PaddingX = 0 : 0;
		( m_PaddingY < 0 ) ? m_PaddingY = 0 : 0;
	}

	if( ImGui::BeginCombo( "Pivot", currentPivot ) )
	{
		for( int i = 3; i < 13; i++ )
		{
			bool isSelected = ( currentPivot == items[i] );
			if( ImGui::Selectable( items[i], isSelected ) )
				currentPivot = items[i];
			if( isSelected )
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}
	if( currentPivot == items[12] )
	{
		ImGui::Columns( 3 );
		ImGui::Text( "Pivot" ); ImGui::NextColumn();
		ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragFloat( "##PivotX", &m_PivotX, 0.01f ); ImGui::NextColumn();
		ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragFloat( "##PivotY", &m_PivotY, 0.01f ); ImGui::NextColumn();
		ImGui::Columns( 1 );
	}

	if( ImGui::Button( "Slice" ) && m_Image.type == fileTypes::image )
	{
		std::string fullPath{ "../Data/" + m_Image.path.string() };
		std::ofstream file{ fullPath + ".spr", std::ios::out | std::ios::binary };
		SDL_Surface* pImage = IMG_Load( fullPath.c_str() );
		if( currentItem == items[1] )
		{
			m_CountX = ( pImage->w - m_OffsetX * 2 + m_PaddingX ) / ( m_Widht + m_PaddingX );
			m_CountY = ( pImage->h - m_OffsetY * 2 + m_PaddingY ) / ( m_Height + m_PaddingY );
		}
		else if( currentItem == items[2] )
		{
			m_Widht = ( pImage->w - m_OffsetX * 2 - ( m_CountX ? m_CountX - 1 : 0 ) * m_PaddingX ) / m_CountX;
			m_Height = ( pImage->h - m_OffsetY * 2 - ( m_CountY ? m_CountY - 1 : 0 ) * m_PaddingY ) / m_CountY;
		}
		sprites.clear();
		BinaryReadWrite::Write( file, m_CountX * m_CountY * 2 );
		for( int y = 0; y < m_CountY; y++ )
		{
			for( int x = 0; x < m_CountX; x++ )
			{
				Vector2 topLeft{ float( m_OffsetX + x * ( m_Widht + m_PaddingX ) ) / float( pImage->w ), float( m_OffsetY + y * ( m_Height + m_PaddingY ) ) / float( pImage->h ) };
				Vector2 bottomRight{ topLeft.x + m_Widht / float( pImage->w ), topLeft.y + m_Height / float( pImage->h ) };
				sprites.push_back( topLeft );
				sprites.push_back( bottomRight );
				BinaryReadWrite::Write( file, topLeft );
				BinaryReadWrite::Write( file, bottomRight );
			}
		}
	}
	ImGui::EndChild();
}

void Balbino::SpriteEditor::DrawImageSlicer( std::vector<Vector2>& sprites )
{
	ImGui::BeginChild( "Image", ImVec2{ -1,-1 }, true );
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;
	if( m_Image.type == fileTypes::image )
	{
		GLuint texture = ResourceManager::LoadTexture( m_Image.path.string() );
		int w{}, h{};
		int miplevel{};
		glBindTexture( GL_TEXTURE_2D, texture );
		glGetTexLevelParameteriv( GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w );
		glGetTexLevelParameteriv( GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h );
		glBindTexture( GL_TEXTURE_2D, 0 );
		ImGui::Image( (void*) (intptr_t) ( texture ), { float( w ), float( h ) } );
		for( int i = 0; i < int( sprites.size() ); i += 2 )
		{
			ImGui::GetForegroundDrawList()->AddRect( { vMin.x + sprites[i].x * w, vMin.y + sprites[i].y * h }, { vMin.x + sprites[i + 1].x * w, vMin.y + sprites[i + 1].y * h }, IM_COL32( 128, 255, 0, 255 ) );
		}
	}
	else
	{
		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImGui::SetCursorPos( { ( vMax.x - vMin.x ) / 2.f - 7.5f, ( vMax.y - vMin.y ) / 2.f } );
		ImGui::Text( "Drop Image Here" );
		ImGui::SetCursorPos( cursorPos );
	}

	ImGui::EndChild();
	if( ImGui::BeginDragDropTarget() )
	{
		if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "asset" ) )
		{
			file droppedFile = *(const file*) payload->Data;
			if( droppedFile.type == fileTypes::image )
			{
				m_Image = droppedFile;
			}
		}
		ImGui::EndDragDropTarget();
	}
}
