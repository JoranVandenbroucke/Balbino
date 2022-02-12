#pragma once
#include <array>

#include "INode.h"

class CVertexOutputNode final : public INode
{
public:
	explicit CVertexOutputNode( int id, int& attributeStartId );
	~CVertexOutputNode() override = default;
	CVertexOutputNode( const CVertexOutputNode& ) = default;
	CVertexOutputNode( CVertexOutputNode&& ) = delete;
	CVertexOutputNode& operator=( const CVertexOutputNode& ) = delete;
	CVertexOutputNode& operator=( CVertexOutputNode&& ) = delete;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType = EAttributeType::None ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_position{}, m_color{};
	std::array<float, 3> m_dummy{ 0.0f, 0.0f, 0.0f };
};
