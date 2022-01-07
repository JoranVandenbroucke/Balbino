#include "pch.h"
#include "NormalMap.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CNormalMap::CNormalMap( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connections{}
	, m_strength{  }
	, m_normal{}
{
	attributeStartId += 3;
}

CNormalMap::~CNormalMap()
{
}

void CNormalMap::Draw()
{
	ImNodes::BeginNode( m_id );
	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Normal Map" );
	ImNodes::EndNodeTitleBar();

	DrawInputFloatAttribute( m_strength, m_attributeStartId, m_connections[0], "Strength", 0, 1 );
	DrawInputVectorAttribute( m_normal, m_attributeStartId + 1, false, "Normal" );

	ImGui::Spacing();
	DrawOutputVectorAttribute( m_attributeStartId + 2, "Normal" );
	ImNodes::EndNode();
}

void CNormalMap::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connections[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connections[1] = true;
}

void CNormalMap::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connections[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connections[1] = false;
}

void CNormalMap::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//vec3 Normal( float strength, vec3 normal, vec4 tangent, vec3 texnormal )
	//todo add include
	if ( attributeType == EAttributeType::Float )
		output << "( ";

	output << "Normal( ";
	if ( m_connections[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_strength );
	output << ", normal, tangent, ";
	if ( m_connections[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << std::to_string( m_strength );
	output << " )";

	if ( attributeType == EAttributeType::Float )
		output << " ).x";
}

bool CNormalMap::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connections[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connections[1];
	return false;
}

int CNormalMap::GetId() const
{
	return m_id;
}

std::vector<int> CNormalMap::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1 };
}
