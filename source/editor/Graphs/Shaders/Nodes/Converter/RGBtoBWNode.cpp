#include "RGBtoBWNode.h"
#include "../Attribute.h"

CRGBtoBWNode::CRGBtoBWNode( int& id )
    : CShaderNode { id, { SSocketType { .type = SSocketType::var_type::color, .name = "colour", .uiName = "Colour" } }, { SSocketType { .type = SSocketType::var_type::float_type, .name = "bw", .uiName = "Black/White" } } }
{
}
void CRGBtoBWNode::Draw() noexcept
{
    StartNode( "RGB to BW", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CRGBtoBWNode::GetCode() const
{
    return "color_rgb_to_bw(colour, bw);";
}
void CRGBtoBWNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_color.glsl" );
}
