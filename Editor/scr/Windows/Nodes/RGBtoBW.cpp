#include "pch.h"
#include "RGBtoBW.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CRGBtoBW::CRGBtoBW( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connections{ false }
	, m_color{}
{
	attributeStartId += 2;
}

CRGBtoBW::~CRGBtoBW()
{
}

void CRGBtoBW::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "RGB To BW" );
	ImNodes::EndNodeTitleBar();

	DrawInputColorAttribute( m_color, m_attributeStartId, m_connections );

	ImGui::Spacing();
	DrawOutputFloatAttribute( m_attributeStartId + 1 );

	ImNodes::EndNode();
}

void CRGBtoBW::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connections = true;
}

void CRGBtoBW::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connections = false;
}

void CRGBtoBW::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//todo add include
	if ( attributeType != EAttributeType::Float )
		output << "vec3( ";
	output << "RGBtoBW( ";
	if(m_connections)
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << "vec3( " << std::to_string( m_color[0] ) << ", " << std::to_string( m_color[0] ) << ", " << std::to_string( m_color[0] ) << " )";

	output << " )";
	if ( attributeType != EAttributeType::Float )
		output << " )";
}

bool CRGBtoBW::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connections;
	return false;
}

int CRGBtoBW::GetId() const
{
	return m_id;
}

std::vector<int> CRGBtoBW::GetInputs() const
{
	return { m_attributeStartId };
}
