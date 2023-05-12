//
// Created by joran on 12/03/2023.
//

#ifndef ENGINE_COLORMIX_H
#define ENGINE_COLORMIX_H
#include <unordered_set>
#include <set>
#include "../ShaderNode.h"

class CColorMixNode final : public CShaderNode
{
public:
    CColorMixNode( int& id );
    ~CColorMixNode() override = default;
    CColorMixNode( const CColorMixNode& ) = delete;
    CColorMixNode( CColorMixNode&& ) = delete;
    CColorMixNode& operator=( const CColorMixNode& ) = delete;
    CColorMixNode& operator=( CColorMixNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
private:
    enum MixMode
    {
        add,
        subtract,
        multiply,
        screen,
        divide,
        difference,
        darken,
        lighten,
        overlay,
        color_dodge,
        color_burn,
        hue,
        saturation,
        value,
        color_mix,
        soft_light,
        linear_light,
        max
    };
    MixMode m_type{ MixMode::color_mix };
    std::vector<std::string> m_allModeNames;
public:
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
private:
    
    const char* ToString( CColorMixNode::MixMode type );
};

#endif //ENGINE_COLORINVERTNODE_H
