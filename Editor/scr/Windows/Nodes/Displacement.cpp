#include "pch.h"
#include "Displacement.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CDisplacement::CDisplacement( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connections{}
	, m_height{ 0 }
	, m_midLevel{ 0 }
	, m_strength{ 0 }
	, m_normal{0,1,0}
{
	attributeStartId += 5;
}

CDisplacement::~CDisplacement()
{
}

void CDisplacement::Draw()
{
	ImNodes::BeginNode( m_id );
	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Displacement Map" );
	ImNodes::EndNodeTitleBar();

	DrawInputFloatAttribute( m_strength, m_attributeStartId, m_connections[0], "Height", 0, 1 );
	DrawInputFloatAttribute( m_strength, m_attributeStartId + 1, m_connections[1], "Mid Level", 0, 1 );
	DrawInputFloatAttribute( m_strength, m_attributeStartId + 2, m_connections[2], "Strength" );
	DrawInputVectorAttribute( m_normal, m_attributeStartId + 3, false, "Normal" );

	ImGui::Spacing();
	DrawOutputVectorAttribute( m_attributeStartId + 4, "Normal" );
	ImNodes::EndNode();
}

void CDisplacement::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connections[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connections[1] = true;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connections[2] = true;
	else if ( m_attributeStartId + 3 == endAttr )
		m_connections[3] = true;
}

void CDisplacement::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connections[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connections[1] = false;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connections[2] = false;
	else if ( m_attributeStartId + 3 == endAttr )
		m_connections[3] = false;
}

void CDisplacement::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//vec3 Displacement( float height, float midlevel, float scale, vec3 normal )
	//todo add include
	if ( attributeType == EAttributeType::Float )
		output << "( ";

	output << "Displacement( ";
	if ( m_connections[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_height );
	output << ", ";
	if ( m_connections[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_midLevel );
	output << ", ";
	if ( m_connections[2] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_strength );
	output << ", ";
	if ( m_connections[3] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "normal";
	output << " )";

	if ( attributeType == EAttributeType::Float )
		output << " ).x";
}

bool CDisplacement::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connections[0];
	else if ( m_attributeStartId + 1 == endAttr )
		return m_connections[1];
	else if ( m_attributeStartId + 2 == endAttr )
		return m_connections[2];
	else if ( m_attributeStartId + 3 == endAttr )
		return m_connections[3];
	return false;
}

int CDisplacement::GetId() const
{
	return m_id;
}

std::vector<int> CDisplacement::GetInputs() const
{
	return { m_attributeStartId ,m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3};
}
