#include "pch.h"
#include "Mapping.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CMapping::CMapping( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connected{}
	, m_vector{}
	, m_position{}
	, m_rotation{}
	, m_scale{}
{
	attributeStartId += 5;
}

CMapping::~CMapping()
{
}

void CMapping::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( ToString( m_type ) );
	ImNodes::EndNodeTitleBar();
	{
		if ( ImGui::BeginCombo( "##Mode", ToString( m_type ) ) )
		{
			for ( int n = 0; n < ( int ) EMode::MaxIndex; n++ )
			{
				const bool isSelected = ( ( int ) m_type == n );
				if ( ImGui::Selectable( ToString( EMode( n ) ), isSelected ) )
					m_type = EMode( n );

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if ( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	DrawInputVectorAttribute( m_vector, m_attributeStartId, false, "Vector" );
	DrawInputVectorAttribute( m_position, m_attributeStartId + 1, m_connected[1], "Color" );
	DrawInputVectorAttribute( m_rotation, m_attributeStartId + 2, m_connected[2], "Color" );
	DrawInputVectorAttribute( m_scale, m_attributeStartId + 3, m_connected[3], "Color" );

	ImGui::Spacing();
	DrawOutputVectorAttribute( m_attributeStartId + 4 );

	ImNodes::EndNode();
}

void CMapping::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
	if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = true;
	if ( m_attributeStartId + 3 == endAttr )
		m_connected[3] = true;
}

void CMapping::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
	if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = false;
	if ( m_attributeStartId + 3 == endAttr )
		m_connected[3] = false;
}

void CMapping::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//todo add include
	if ( attributeType == EAttributeType::Float )
		output << "( ";
	switch ( m_type )
	{
		case Point:
			output << "MappingPoint( ";
			break;
		case Texture:
			output << "MappingTexture( ";
			break;
		case Vector:
			output << "MappingVector( ";
			break;
		case Normal:
			output << "MappingNormal( ";
			break;
	}

	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_vector[0] ) << ", " << std::to_string( m_vector[1] ) << ", " << std::to_string( m_vector[2] ) << ")";

	output << ", ";
	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_position[0] ) << ", " << std::to_string( m_position[1] ) << ", " << std::to_string( m_position[2] ) << ")";

	output << ", ";
	if ( m_connected[2] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_rotation[0] ) << ", " << std::to_string( m_rotation[1] ) << ", " << std::to_string( m_rotation[2] ) << ")";

	output << ", ";
	if ( m_connected[3] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_scale[0] ) << ", " << std::to_string( m_scale[1] ) << ", " << std::to_string( m_scale[2] ) << ")";

	output << " )";
	if ( attributeType == EAttributeType::Float )
		output << " ).x";
}

bool CMapping::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connected[1];
	if ( m_attributeStartId + 2 == endAttr )
		return m_connected[2];
	if ( m_attributeStartId + 3 == endAttr )
		return m_connected[3];
	return false;
}

int CMapping::GetId() const
{
	return m_id;
}

std::vector<int> CMapping::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3 };
}

const char* CMapping::ToString( EMode type )
{
	switch ( type )
	{
		case Point:
			return "Point";
		case Texture:
			return "texture";
		case Vector:
			return "Vector";
		case Normal:
			return "Normal";
	}
	return "";
}
