#include "pch.h"
#include "Bump.h"

#include <string>

#include "imgui.h"
#include "imnodes.h"

#include "Attribute.h"

CBump::CBump( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_invert{ false }
	, m_connections{}
	, m_strength{ 1 }
	, m_distance{ 1 }
	, m_height{ 0 }
	, m_normal{0,1,0}
{
	attributeStartId += 5;	
}

CBump::~CBump()
= default;

void CBump::Draw()
{
	ImNodes::BeginNode( m_id );
	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Bump Node" );
	ImNodes::EndNodeTitleBar();

	ImGui::Checkbox( "invert", &m_invert );

	DrawInputFloatAttribute( m_strength, m_attributeStartId, m_connections[0], "Strength", 0, 1 );
	DrawInputFloatAttribute( m_distance, m_attributeStartId + 1, m_connections[1], "Distance", 0, 1 );
	DrawInputFloatAttribute( m_height, m_attributeStartId + 2, false, "Height", 0, 1 );
	DrawInputVectorAttribute( m_normal, m_attributeStartId + 3, false, "Normal" );

	ImGui::Spacing();
	DrawOutputVectorAttribute( m_attributeStartId + 4, "Normal" );
	ImNodes::EndNode();
}

void CBump::Attach( int endAttr )
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

void CBump::Detach( int endAttr )
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

void CBump::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;

	//todo add include
	//vec3 Bump( bool invert, float strength, float dist, float height, vec3 normal, vec3 position )
	if ( attributeType == EAttributeType::Float )
		output << "( ";
	output << "Bump( " << std::to_string(m_invert) << ", ";
	if( m_connections[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string(m_strength);

	output << ", ";
	if ( m_connections[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_distance );

	output << ", ";
	if ( m_connections[2] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_height );

	output << ", ";
	if ( m_connections[3] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "normal";

	output << ", fragWorldPosition.xyz )";

	if ( attributeType == EAttributeType::Float )
		output << " ).x";
}

bool CBump::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connections[0];

	if ( m_attributeStartId + 1 == endAttr )
		return m_connections[1];

	if ( m_attributeStartId + 2 == endAttr )
		return m_connections[2];

	if ( m_attributeStartId + 3 == endAttr )
		return m_connections[3];
	return false;
}

int CBump::GetId() const
{
	return m_id;
}

std::vector<int> CBump::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3 };
}
