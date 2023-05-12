#pragma once
#include <array>
#include <set>

#include "../ShaderNode.h"

class CMappingNode final : public CShaderNode
{
public:
	CMappingNode( int& id );
    ~CMappingNode() override = default;
    CMappingNode( const CMappingNode& ) = delete;
    CMappingNode( CMappingNode&& ) = delete;
    CMappingNode& operator=( CMappingNode&& ) = delete;
    CMappingNode& operator=( const CMappingNode& ) = delete;

	void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
	enum mapping_type
	{
        mapping_type_point,
        mapping_type_texture,
        mapping_type_vector,
        mapping_type_normal,
        mapping_type_max
	};
    mapping_type             m_type{ mapping_type_point};
    std::vector<std::string> m_allNames;
	static const char* ToString( mapping_type type );
};
