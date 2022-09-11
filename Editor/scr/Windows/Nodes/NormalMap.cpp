#include "NormalMap.h"

#include <string>
#include <format>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CNormalMap::CNormalMap( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connections{}
	, m_strength{}
	, m_normal{}
{
	attributeStartId += 3;
}

CNormalMap::~CNormalMap()
= default;

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

std::string CNormalMap::Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType )
{
	( void ) begin;
	( void ) bindings;
	( void ) attributeType;
	//vec3 Normal( float strength, vec3 normal, vec4 tangent, vec3 texnormal )
    std::string shader;
    includes.insert("normal.glsl");

    shader += std::format("Normal({},fragTangent, fragNormal,{})"
            ,m_connections[0]? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ):std::to_string(m_strength)
            ,m_connections[1]? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ):"vec3("+std::to_string(m_normal[0])+","+std::to_string(m_normal[1])+","+std::to_string(m_normal[2])+")");

	if ( attributeType == EAttributeType::Float )
        shader += ".x";
    return shader;
}

bool CNormalMap::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return !m_connections[0];
	if ( m_attributeStartId + 1 == endAttr )
		return !m_connections[1];
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
