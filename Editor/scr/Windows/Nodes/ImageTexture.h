//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#ifndef GAME_IMAGETEXTURE_H
#define GAME_IMAGETEXTURE_H

#include "INode.h"

namespace BalEditor
{
    class CImageTexture final : public INode
    {
    public:
        CImageTexture( int id, int& attributeStartId );

        ~CImageTexture() override = default;

        CImageTexture( const CImageTexture& ) = delete;

        CImageTexture( CImageTexture&& ) = delete;

        CImageTexture& operator =( CImageTexture&& ) = delete;

        CImageTexture& operator =( const CImageTexture& ) = delete;

        void Draw() override;

        void Attach( int endAttr ) override;

        void Detach( int endAttr ) override;

        std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType ) override;

        bool HasFreeAttachment( int endAttr ) const override;

        int GetId() const override;

        std::vector<int> GetInputs() const override;

    private:
        enum class ETextureType
        {
            //todo Texture1D,
            Texture2D,
            //todo Texture3D,
            //todo TextureCube,
            //todo Texture2DRect,
            //todo Texture1DArray,
            //todo Texture2DArray,
#if version > 400
            //todo TextureCubeArray,
#endif
            //todo TextureBuffer,
            //todo Texture2DMS,
            //todo Texture2DMSArray,
            MaxIndex
        }           m_textureType;
        std::string m_textureVariableName;

        //todo: allow for custom UVs and Lod/Grad sampling

        static const char* ToString( ETextureType mode );
    };

} // BalEditor

#endif //GAME_IMAGETEXTURE_H
