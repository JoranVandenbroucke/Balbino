//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#pragma once
#include "../ShaderNode.h"
#include <set>
#include <unordered_set>

class CColorMixNode final : public CShaderNode
{
public:
    explicit CColorMixNode( int& id );
    ~CColorMixNode() override                        = default;
    CColorMixNode( const CColorMixNode& )            = delete;
    CColorMixNode( CColorMixNode&& )                 = delete;
    CColorMixNode& operator=( const CColorMixNode& ) = delete;
    CColorMixNode& operator=( CColorMixNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
    enum mix_mode
    {
        mix_mode_add,
        mix_mode_subtract,
        mix_mode_multiply,
        mix_mode_screen,
        mix_mode_divide,
        mix_mode_difference,
        mix_mode_darken,
        mix_mode_lighten,
        mix_mode_overlay,
        mix_mode_color_dodge,
        mix_mode_color_burn,
        mix_mode_hue,
        mix_mode_saturation,
        mix_mode_value,
        mix_mode_color_mix,
        mix_mode_soft_light,
        mix_mode_linear_light,
        mix_mode_max
    };
    mix_mode m_type { mix_mode::mix_mode_color_mix };
    std::vector<std::string> m_allModeNames;

private:
    const char* ToString( CColorMixNode::mix_mode type ) const;
};
