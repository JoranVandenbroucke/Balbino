//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#include <format>
#include "ImageTexture.h"

#include "../Attribute.h"
#include "../../../EditorGUI/EditorGui.h"


CImageTexture::CImageTexture( int id, int& attributeStartId )
        : INode( id, attributeStartId )
        , m_textureVariableName{ "defaultTexture" }
        , m_type{ EMode::Texture2D }
        , m_uv{ 0, 0, 0 }
        , m_useUV{}
{
    attributeStartId += 2;
    m_vertexFlags = 1 << 1;
}

void CImageTexture::Draw()
{
    ImNodes::BeginNode( m_id, 350.f );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "Texture" );
    ImNodes::EndNodeTitleBar();
    char      name[64]{};
    for ( int i{}; i < m_textureVariableName.size(); ++i )
    {
        name[i] = m_textureVariableName[i];
    }
    
    if ( FawnForge::GUI::DrawInputText( "Texture Variable Name", name, 64, 175 ))
    {
        m_textureVariableName = "";
        for ( int i{}; i < 64; ++i )
        {
            if ( name[i] == '\0' )
            {
                break;
            }
            m_textureVariableName += name[i];
        }
    }
    
    {
        uint64_t                 type{ (uint64_t) m_type };
        std::vector<std::string> types;
        for ( int                i{}; i < int( EMode::MaxIndex ); ++i )
        {
            types.emplace_back( ToString( EMode( i )));
        }
        
        FawnForge::GUI::DrawComboBox( "Mode", type, types, {}, 175 );
        m_type =(EMode)type;
    }
    DrawInputVectorAttribute( m_uv, m_attributeStartId, true, "UV" );
    
    FawnForge::GUI::Spacing();
    DrawOutputTextureAttribute( m_attributeStartId + 1 );
    //todo add options to overwrite auto binding and set
    ImNodes::EndNode();
}

void CImageTexture::Attach( int endAttr )
{
    if ( endAttr == m_attributeStartId )
    {
        m_useUV       = true;
        m_vertexFlags = 0;
    }
}

void CImageTexture::Detach( int endAttr )
{
    if ( endAttr == m_attributeStartId )
    {
        m_useUV       = false;
        m_vertexFlags = 1 << 1;
    }
}

std::string CImageTexture::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) includes;
    (void) attributeType;
    std::string shader;
    //todo: allow for custom Lod/Grad/Project sampling
    switch ( m_type )
    {
        case EMode::Texture2D:
            bindings.insert( "sampler2D " + m_textureVariableName );
            shader = std::format(
                    "GetTexture2D({},{}).xyz",
                    m_textureVariableName,
                    m_useUV ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )+".xy" : "fragTexCoord"
            );
            break;
        case EMode::Texture3D:
            bindings.insert( "sampler3D " + m_textureVariableName );
            shader = std::format(
                    "GetTexture3D({},{})",
                    m_textureVariableName,
                    m_useUV ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : "fragTexCoord"
            );
            break;
        case EMode::TextureCube:
            bindings.insert( "samplerCube " + m_textureVariableName );
            shader = std::format(
                    "GetTextureCube({},{}).xyz",
                    m_textureVariableName,
                    m_useUV ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : "fragTexCoord"
            );
            break;
        case EMode::Texture2DArray:
            bindings.insert( "sampler2DArray " + m_textureVariableName );
            shader = std::format(
                    "GetTexture2DArray({},{}).xyz",
                    m_textureVariableName,
                    m_useUV ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : "fragTexCoord"
            );
            break;
        case EMode::MaxIndex:
            break;
    }
    return shader;
}

bool CImageTexture::HasFreeAttachment( int endAttr ) const
{
    (void) endAttr;
    if ( endAttr == m_attributeStartId )
    {
        return m_useUV;
    }
    return false;
}

int CImageTexture::GetId() const
{
    return m_id;
}

std::vector<int> CImageTexture::GetInputs() const
{
    return { m_attributeStartId };
}

const char* CImageTexture::ToString( EMode mode )
{
    switch ( mode )
    {
        case EMode::Texture2D:
            return "Texture 2D";
        case EMode::TextureCube:
            return "Texture Cube";
        case EMode::Texture2DArray:
            return "Texture 2D Array";
        case EMode::Texture3D:
            return "Texture 3D";
        case EMode::MaxIndex:
            break;
    }
    return nullptr;
}
uint8_t CImageTexture::GetVertexFlags() const
{
    return m_vertexFlags;
}
