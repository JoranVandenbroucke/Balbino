#pragma once
#include <array>

#include "INode.h"

class CBrightContrast final : public INode
{
public:
	CBrightContrast( const int id, int& attributeStartId );
	~CBrightContrast() override;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connected[3];
	std::array<float, 3> m_color;
	float m_brightness;
	float m_contrast;
};
class CGamma final : public INode
{
public:
	CGamma( const int id, int& attributeStartId );
	~CGamma() override;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connected[2];
	std::array<float, 3> m_color;
	float m_gamma;
};
class CHueSaturationValue final: public INode
{
public:
	CHueSaturationValue( const int id, int& attributeStartId );
	~CHueSaturationValue() override;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connected[5];
	std::array<float, 3> m_color;
	float m_hue;
	float m_saturation;
	float m_value;
	float m_factor;
};
class CInvert final : public INode
{
public:
	CInvert( const int id, int& attributeStartId );
	~CInvert() override;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connected[2];
	std::array<float, 3> m_color;
	float m_factor;
};
class CMix final : public INode
{
public:
	CMix( const int id, int& attributeStartId );
	~CMix() override;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	enum class EMode
	{
		Add,
		Subtract,
		Multiply,
		Screen,
		Divide,
		Difference,
		Darken,
		Lighten,
		Overlay,
		ColorDodge,
		ColorBurn,
		Hue,
		Saturation,
		Value,
		ColorMix,
		SoftLight,
		LinearLight,
		MaxIndex
	}m_type;
	bool m_connected[3];
	std::array<float, 3> m_color1;
	std::array<float, 3> m_color2;
	float m_factor;
	static const char* ToString( EMode mode );
};