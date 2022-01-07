#pragma once
#include "INode.h"

#include <array>
class CRGBtoBW final : public INode
{
public:
	CRGBtoBW( const int id, int& attributeStartId );

	~CRGBtoBW() override;
	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connections;
	std::array<float, 3> m_color;
};

