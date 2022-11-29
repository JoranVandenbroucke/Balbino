#pragma once
#include <array>

#include "INode.h"

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
	std::string Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	/*
	 *  1.  color baseColor .82 .67 .16
	 *  2.  float metallic 0 1 0
	 *  3.  float subsurface 0 1 0
	 *  4.  float specular 0 1 .5
	 *  5.  float roughness 0 1 .5
	 *  6.  float specularTint 0 1 0
	 *  7.  float anisotropic 0 1 0
	 *  8.  float sheen 0 1 0
	 *  9.  float sheenTint 0 1 .5
	 *  10. float clearCoat 0 1 0
	 *  11. float clearCoatGloss 0 1 1
	 *  12. float alpha 0 1 1
	 *  13. vector normal 0 1 0
	 */
	static constexpr int m_connectionSize = 14;
	bool m_connections[m_connectionSize];
	std::array<float, 3> m_color{ 0.82f, 0.67f, 0.16f };
	float m_metallic{ 0 };
	float m_subsurface{ 0 };
	float m_specular{ 0.5f };
	float m_roughness{ 0.5f };
	float m_specularTint{ 0.0f };
	float m_anisotropic{ 0.0f };
	float m_sheen{ 0.0f };
	float m_sheenTint{ 0.5f };
	float m_clearCoat{ 0.0f };
	float m_clearCoatGloss{ 0.1f };
	float m_alpha{ 1.0f };
	std::array<float, 3> m_normal{ 0.0f, 1.0f, 0.0f };
};
