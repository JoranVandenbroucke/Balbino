#pragma once
#include <array>
#include <unordered_set>

#include "../ShaderNode.h"

class CFragmentOutputNode final : public CShaderNode
{
public:
    explicit CFragmentOutputNode( int& id );
    ~CFragmentOutputNode() override = default;
    CFragmentOutputNode( const CFragmentOutputNode& ) = default;
    CFragmentOutputNode( CFragmentOutputNode&& ) = delete;
    CFragmentOutputNode& operator=( const CFragmentOutputNode& ) = delete;
    CFragmentOutputNode& operator=( CFragmentOutputNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
    void SetFlag( SSocketType::compiler_define flags );
private:
    enum diffuse : uint8_t
    {
        // BRDF flags
        diffuse_lambert,
        diffuse_burley,
        diffuse_oren_nayar,
        diffuse_max
    };
    enum distribution : uint8_t
    {
        distribution_blin_phong,
        distribution_beckmann,
        distribution_ggx,
        distribution_max
    };
    enum specular : uint8_t
    {
        // Visibility flags
        specular_implicit,
        specular_neumann,
        specular_cooktorrance,
        specular_kelemen,
        specular_ggx_correlated,
        specular_ggx_correlated_fast,
        specular_kelemen_two,
        specular_neubelt,
        specular_max
    };
    enum fresnel : uint8_t
    {
        // Fresnel functions
        fresnel_cook_torrance,
        fresnel_schlick,
        fresnel_max
    };
    
    uint32_t m_flags{};
    std::array<std::string, diffuse_max> m_diffuseNames;
    std::array<std::string, distribution_max> m_distributionNames;
    std::array<std::string, specular_max> m_specularNames;
    std::array<std::string, fresnel_max> m_fresnelNames;
    
    diffuse      m_diffuse{ diffuse_burley };
    distribution m_distribution{ distribution_ggx };
    specular     m_specular{ specular_ggx_correlated_fast };
    fresnel      m_fresnel{ fresnel_schlick };
    
    static const char* ToString(diffuse type);
    static const char* ToString(distribution type);
    static const char* ToString(specular type);
    static const char* ToString(fresnel type);
    
    static const char* ToDefine(diffuse type);
    static const char* ToDefine(distribution type);
    static const char* ToDefine(specular type);
    static const char* ToDefine(fresnel type);
};
