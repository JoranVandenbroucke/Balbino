#include "RGBtoBWNode.h"
#include "../Attribute.h"

CRGBtoBWNode::CRGBtoBWNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "colour", "Colour" }}, {
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "bw", "Black/White" }}}
{
}
void CRGBtoBWNode::Draw()
{
    StartNode( "RGB to BW", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CRGBtoBWNode::GetCode() const
{
    return "color_rgb_to_bw(colour, bw);";
}
void CRGBtoBWNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_color.glsl");
}
