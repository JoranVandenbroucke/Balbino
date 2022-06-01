#include "VertexOutput.h"

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CVertexOutputNode::CVertexOutputNode( int id, int& attributeStartId )
	: INode{ id, attributeStartId }
{
	attributeStartId += 2;
}

void CVertexOutputNode::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted( "Vertex Output" );
	ImNodes::EndNodeTitleBar();

	//position
	DrawInputVectorAttribute( m_positionFloats, m_attributeStartId, true, ":position" );

	//color
	DrawInputColorAttribute( m_colorFloats, m_attributeStartId + 1, true, ":color" );

	ImNodes::EndNode();
}

void CVertexOutputNode::Attach( int endAttr )
{
	if ( endAttr == m_attributeStartId )
		m_position = true;
	if ( endAttr == m_attributeStartId + 1 )
		m_color = true;
}

void CVertexOutputNode::Detach( int endAttr )
{
	if ( endAttr == m_attributeStartId )
		m_position = false;
	if ( endAttr == m_attributeStartId + 1 )
		m_color = false;
}

std::string CVertexOutputNode::Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType )
{
	( void ) begin;
    ( void ) bindings;
    ( void ) includes;
    ( void ) attributeType;
    std::string shader{ R"(#version 450

layout(set=0, binding=0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec4 inTangent;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec4 fragWorldPos;

void main()
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0f);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = normalize( mat3(ubo.model) * inNormal);
    fragTangent = normalize( mat3(ubo.model) * inTangent.xyz) * inTangent.w;
    fragWorldPos = ubo.model * vec4(inPosition, 1.0f);
})"};
    return shader;
}

bool CVertexOutputNode::HasFreeAttachment( int endAttr ) const
{
	if ( !m_position && endAttr == m_attributeStartId )
		return true;
	if ( !m_color && endAttr == m_attributeStartId + 1 )
		return true;
	return false;
}

int CVertexOutputNode::GetId() const
{
	return m_id;
}

std::vector<int> CVertexOutputNode::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1 };
}
