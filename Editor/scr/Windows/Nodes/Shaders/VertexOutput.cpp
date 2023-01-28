#include "VertexOutput.h"

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CVertexOutputNode::CVertexOutputNode( int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_useCameraTransform{ true }
        , m_inputFlags{}
        , m_position{}
        , m_color{}
        , m_normal{}
        , m_tangent{}
        , m_uv{}
        , m_connections{}
{
    attributeStartId += 6;
}

void CVertexOutputNode::Draw()
{
    ImNodes::BeginNode( m_id );
    BalEditor::GUI::DrawToggle( "Use Camera Transform", m_useCameraTransform );
    DrawInputVectorAttribute( m_position, m_attributeStartId, false, "Positions" );
    BalEditor::GUI::SetTooltip( "This will add to the original vertex position." );
    
    if ( 0b00000001 & m_vertexFlags )
    {
        DrawInputColorAttribute( m_color, m_attributeStartId + 1, m_connections[1], "Colour" );
        BalEditor::GUI::SetTooltip( "This will override the original vertex colour." );
    }
    if ( 0b00000010 & m_vertexFlags )
    {
        DrawInputVectorAttribute( m_uv, m_attributeStartId + 2, m_connections[2], "UV" );
        BalEditor::GUI::SetTooltip( "This will override the original vertex UVs." );
    }
    DrawInputVectorAttribute( m_normal, m_attributeStartId + 3, true, "Normal" );
    BalEditor::GUI::SetTooltip( "This will override the original vertex Normals." );
    if ( 0b00000100 & m_vertexFlags )
    {
        DrawInputVectorAttribute( m_tangent, m_attributeStartId + 4, false, "Tangent" );
        BalEditor::GUI::SetTooltip( "This will override the original vertex Tangents." );
    }
    DrawOutputShaderAttribute( "Output:", m_attributeStartId + 5 );
    ImNodes::EndNode();
}

void CVertexOutputNode::Attach( int endAttr )
{
    if ( endAttr < m_attributeStartId || endAttr > m_attributeStartId + 4 )
    {
        return;
    }
    int id = endAttr - m_attributeStartId;
    m_inputFlags += 1 << id;
}

void CVertexOutputNode::Detach( int endAttr )
{
    if ( endAttr < m_attributeStartId || endAttr > m_attributeStartId + 4 )
    {
        return;
    }
    int id = endAttr - m_attributeStartId;
    m_inputFlags -= 1 << id;
}

std::string CVertexOutputNode::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) includes;
    (void) attributeType;
    std::string vert{
            m_connections[0] ? "inPosition + " + ( *( begin++ ))->Evaluate(
                    begin, bindings, includes, EAttributeType::Vector
            ) : "inPosition"
    };
    
    size_t idx = m_calculationsString.find( "{0}" );
    if ( idx != std::string::npos )
    {
        m_calculationsString.replace( idx, 3, vert );
    }
    idx = m_calculationsString.find( "{0}" );
    if ( idx != std::string::npos )
    {
        m_calculationsString.replace( idx, 3, vert );
    }
    idx = m_calculationsString.find( "{2}" );
    if ( idx != std::string::npos )
    {
        m_calculationsString.replace(
                idx, 3, m_connections[1] ? ( *( begin++ ))->Evaluate(
                        begin, bindings, includes, EAttributeType::Color
                ) : "inColor"
        );
    }
    idx = m_calculationsString.find( "{3}" );
    if ( idx != std::string::npos )
    {
        m_calculationsString.replace(
                idx, 3, m_connections[2] ? ( *( begin++ ))->Evaluate(
                        begin, bindings, includes, EAttributeType::Vector
                ) : "inTexCoord"
        );
    }
    idx = m_calculationsString.find( "{1}" );
    if ( idx != std::string::npos )
    {
        m_calculationsString.replace(
                idx, 3, m_connections[3] ? ( *( begin++ ))->Evaluate(
                        begin, bindings, includes, EAttributeType::Vector
                ) : "inNormal"
        );
    }
    idx = m_calculationsString.find( "{4}" );
    if ( idx != std::string::npos )
    {
        m_calculationsString.replace(
                idx, 3, m_connections[4] ? ( *( begin++ ))->Evaluate(
                        begin, bindings, includes, EAttributeType::Vector
                ) : "inTangent"
        );
    }
    
    std::string shader{
            R"(#version 450
            
{0}

{1}

layout(set=0, binding=0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    vec4 viewPos;
    int displayDebugTarget;
} ubo;

void main()
{
    {2}
})"
    };
    shader.replace(
            shader.find( "{0}" ), 3, m_inputString
    );
    
    shader.replace(
            shader.find( "{1}" ), 3, m_outputString
    );
    shader.replace(
            shader.find( "{2}" ), 3, m_calculationsString
    );
    return shader;
}

bool CVertexOutputNode::HasFreeAttachment( int endAttr ) const
{
    if ( endAttr < m_attributeStartId || endAttr > m_attributeStartId + 4 )
    {
        return false;
    }
    int id = endAttr - m_attributeStartId;
    return !( m_inputFlags & ( 1 << id ));
}

int CVertexOutputNode::GetId() const
{
    return m_id;
}

std::vector<int> CVertexOutputNode::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3 };
}
uint8_t CVertexOutputNode::GetVertexFlags() const
{
    return 0;
}
void CVertexOutputNode::SetVertexFlags( uint8_t flags )
{
    m_vertexFlags        = flags;
    m_inputString        = "layout(location = 0) in vec3 inPosition;\n";
    m_outputString       = "";
    m_calculationsString = R"(    gl_Position = ubo.proj * ubo.view * instanceModelMatrix * vec4({0}, 1.0f);
    fragNormal = normalize(mat3(instanceModelMatrix) * {1});
    fragWorldPos = instanceModelMatrix * vec4({0}, 1.0f);
)";
    if ( 0b00000001 & flags )
    {
        m_inputString += "layout(location = 1) in vec4 inColor;\n";
        m_outputString += "layout(location = 0) out vec4 fragColor;\n";
        m_calculationsString += "fragColor={2};\n";
    }
    if ( 0b00000010 & flags )
    {
        m_inputString += "layout(location = 2) in vec2 inTexCoord;\n";
        m_outputString += "layout(location = 1) out vec2 fragTexCoord;\n";
        m_calculationsString += "fragTexCoord = {3};\n";
    }
    m_inputString += "layout(location = 3) in vec3 inNormal;\n";
    m_outputString += "layout(location = 2) out vec3 fragNormal;\n";
    if ( 0b00000100 & flags )
    {
        m_inputString += "layout(location = 4) in vec4 inTangent;\n";
        m_outputString +=  "layout(location = 3) out vec4 fragTangent;\n";
        m_calculationsString += "fragTangent = normalize(mat3(instanceModelMatrix) * {4}.xyz)*{4}.w;\n";
    }
    m_inputString += "layout(location = 5) in mat4 instanceModelMatrix;\n";
    m_outputString += "layout(location = 4) out vec4 fragWorldPos;\n";
}
