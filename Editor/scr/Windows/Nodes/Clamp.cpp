#include "pch.h"
#include "Clamp.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"

CClamp::CClamp( int id, int& firstAttributeId )
	: INode{ id, firstAttributeId }
	, m_connected{ false }
	, m_value{ 0 }
{
	firstAttributeId += 4;
}

void CClamp::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Clamp" );
	ImNodes::EndNodeTitleBar();

	DrawInputFloatAttribute( m_value[0], m_attributeStartId, m_connected[0] );
	DrawInputFloatAttribute( m_value[1], m_attributeStartId, m_connected[1], ":Min" );
	DrawInputFloatAttribute( m_value[2], m_attributeStartId, m_connected[2], ":Max" );

	ImGui::Spacing();
	DrawOutputFloatAttribute( m_attributeStartId + 3 );

	ImNodes::EndNode();
}

void CClamp::Attach( int endAttr )
{
	if ( m_attributeStartId <= endAttr && m_attributeStartId + 4 > endAttr )
		m_connected[endAttr - m_attributeStartId] = true;
}

void CClamp::Detach( int endAttr )
{
	if ( m_attributeStartId <= endAttr && m_attributeStartId + 4 > endAttr )
		m_connected[endAttr - m_attributeStartId] = false;
}

void CClamp::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;

	if ( attributeType != EAttributeType::Float )
		output << "vec3( ";
	output << "clamp(";
	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_value[0] );

	output << ", ";

	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_value[1] );

	output << ", ";

	if ( m_connected[2] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_value[2] );

	output << ")";
	if ( attributeType != EAttributeType::Float )
		output << ")";
}

bool CClamp::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId <= endAttr && m_attributeStartId + 4 > endAttr )
		return m_connected[endAttr - m_attributeStartId];
	return false;
}

int CClamp::GetId() const
{
	return m_id;
}

std::vector<int> CClamp::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}
