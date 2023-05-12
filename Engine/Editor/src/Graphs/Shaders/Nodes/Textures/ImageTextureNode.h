//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#ifndef GAME_IMAGETEXTURE_H
#define GAME_IMAGETEXTURE_H

#include <set>
#include "../ShaderNode.h"

class CImageTextureNode final : public CShaderNode
{
public:
    CImageTextureNode( int& id );
    ~CImageTextureNode() override = default;
    CImageTextureNode( const CImageTextureNode& ) = delete;
    CImageTextureNode( CImageTextureNode&& ) = delete;
    CImageTextureNode& operator=( CImageTextureNode&& ) = delete;
    CImageTextureNode& operator=( const CImageTextureNode& ) = delete;
    
    void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
private:
    enum texture
    {
        texture_1d,
        texture_2d,
        texture_3d,
        texture_cube,
        texture_2d_rect,
        texture_1d_array,
        texture_2d_array,
        texture_cube_array,
        texture_max
    };
    
    SShaderBinding m_binding{};
    texture m_type{ texture_2d };
    bool    m_isShadow{};
//        bool m_isProjection{};
//        bool m_isOffset{};
//        bool m_isLod{};
//        bool m_isGrad{};
    
    std::string              m_bindingName{ "defaultTexture" };
    std::vector<std::string> m_allModeNames;
    
    //todo: allow for custom UVs and Lod/Grad sampling
    
    static const char* ToString( texture mode );
};


#endif //GAME_IMAGETEXTURE_H
