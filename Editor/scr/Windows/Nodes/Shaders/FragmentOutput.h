#pragma once
#include <array>

#include "../INode.h"

class CFragmentOutputNode final : public INode
{
public:
    CFragmentOutputNode( int id, int& attributeStartId );
    ~CFragmentOutputNode() override = default;
    CFragmentOutputNode( const CFragmentOutputNode& ) = default;
    CFragmentOutputNode( CFragmentOutputNode&& ) = delete;
    CFragmentOutputNode& operator=( const CFragmentOutputNode& ) = delete;
    CFragmentOutputNode& operator=( CFragmentOutputNode&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
    
    void SetVertexFlags(uint8_t flags);
private:
    enum class ENormalDistributionFunction
    {
        BlinnPhong,
        Beckmann,
        GGX
    };
    enum class EShadow
    {
        Implicit,
        Neumann,
        CookTorrance,
        Kelemen,
        Neubelt,
        Beckmann,
        GGXCorrelated,
        GGXCorrelatedFast
    };
    enum class EFresnel
    {
        CookTorrance,
        Schlick
    };
    enum class EDiffuse
    {
        Lambert,
        Burley,
        OrenNayar
    };
    
    ENormalDistributionFunction m_ndf;
    EShadow m_shadow;
    EFresnel m_fresnel;
    EDiffuse m_diffuse;
    
    static constexpr int m_connectionSize = 5;
    bool                 m_connections[m_connectionSize];
    float                m_metallic{ 0 };
    float                m_roughness{ 0.5f };
    float                m_alpha{ 1.0f };
    std::array<float, 3> m_color{ 0.82f, 0.67f, 0.16f };
    std::array<float, 3> m_normal{ 0.0f, 1.0f, 0.0f };
    std::string m_specularString;
    std::string m_shadowString;
    std::string m_fresnelString;
    std::string m_diffuseString;
    std::vector<std::string> m_ndfs;
    std::vector<std::string> m_shadows;
    std::vector<std::string> m_fresnels;
    std::vector<std::string> m_diffuses;
    
    std::string m_inputString;
    
    const char* ToString(ENormalDistributionFunction type);
    const char* ToString(EShadow type);
    const char* ToString(EFresnel type);
    const char* ToString(EDiffuse type);
};
