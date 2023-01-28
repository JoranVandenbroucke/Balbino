//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#ifndef GAME_IMAGETEXTURE_H
#define GAME_IMAGETEXTURE_H

#include <array>
#include "../INode.h"

class CImageTexture final : public INode
{
public:
    CImageTexture( int id, int& attributeStartId );
    ~CImageTexture() override = default;
    CImageTexture( const CImageTexture& ) = delete;
    CImageTexture( CImageTexture&& ) = delete;
    CImageTexture& operator=( CImageTexture&& ) = delete;
    CImageTexture& operator=( const CImageTexture& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;

private:
    enum class EMode
    {
        Texture2D,
        Texture3D,
        TextureCube,
        Texture2DArray,
        MaxIndex
    }           m_type;
    std::string m_textureVariableName;
    
    //todo: allow for custom Lod/Grad sampling
    std::array<float,3> m_uv;
    bool m_useUV;
    static const char* ToString( EMode mode );
};

#endif //GAME_IMAGETEXTURE_H
