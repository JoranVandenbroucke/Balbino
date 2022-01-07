#include "pch.h"
#include "ColorNodes.h"

#include <string>

#include "Attribute.h"
#include "imnodes.h"

CBrightContrast::CBrightContrast( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connected{}
	, m_color{}
	, m_brightness{ 0 }
	, m_contrast{ 0 }
{
	attributeStartId += 4;
}

CBrightContrast::~CBrightContrast()
= default;

void CBrightContrast::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Brightness / Contrast" );
	ImNodes::EndNodeTitleBar();

	DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
	DrawInputFloatAttribute( m_brightness, m_attributeStartId + 1, m_connected[1], "Brightness" );
	DrawInputFloatAttribute( m_contrast, m_attributeStartId + 2, m_connected[2], "Contrast" );

	ImGui::Spacing();
	DrawOutputColorAttribute( m_attributeStartId + 3 );

	ImNodes::EndNode();
}

void CBrightContrast::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = true;
}

void CBrightContrast::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = false;
}

void CBrightContrast::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;

	if ( attributeType == EAttributeType::Float )
		output << "( ";
	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << "vec3( " << std::to_string( m_color[0] ) << ", " << std::to_string( m_color[1] ) << ", " << std::to_string( m_color[2] ) << ")";

	output << " * ";

	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << std::to_string( m_contrast );

	output << " + ";

	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << std::to_string( m_brightness );

	if ( attributeType == EAttributeType::Float )
		output << ").x";
}

bool CBrightContrast::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connected[1];
	if ( m_attributeStartId + 2 == endAttr )
		return m_connected[2];
	return false;
}

int CBrightContrast::GetId() const
{
	return m_id;
}

std::vector<int> CBrightContrast::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}

CGamma::CGamma( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connected{}
	, m_color{}
	, m_gamma{ 0 }
{
	attributeStartId += 3;
}

CGamma::~CGamma()
= default;

void CGamma::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Gamma" );
	ImNodes::EndNodeTitleBar();

	DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
	DrawInputFloatAttribute( m_gamma, m_attributeStartId + 1, m_connected[1], "Gamma" );

	ImGui::Spacing();
	DrawOutputColorAttribute( m_attributeStartId + 2 );

	ImNodes::EndNode();
}

void CGamma::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
}

void CGamma::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
}

void CGamma::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	if ( attributeType == EAttributeType::Float )
		output << "( ";
	output << "pow( ";
	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << "vec3( " << std::to_string( m_color[0] ) << ", " << std::to_string( m_color[1] ) << ", " << std::to_string( m_color[2] ) << ")";

	output << ", ";

	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << std::to_string( m_gamma );

	output << " )";


	if ( attributeType == EAttributeType::Float )
		output << ").x";
}

bool CGamma::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connected[1];
	return false;
}

int CGamma::GetId() const
{
	return m_id;
}

std::vector<int> CGamma::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1 };
}

CHueSaturationValue::CHueSaturationValue( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connected{}
	, m_color{}
	, m_hue{ 0.5f }
	, m_saturation{ 0 }
	, m_value{ 0 }
	, m_factor{ 1 }
{
	attributeStartId += 6;
}

CHueSaturationValue::~CHueSaturationValue()
= default;

void CHueSaturationValue::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Hue Saturation Value" );
	ImNodes::EndNodeTitleBar();

	DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
	DrawInputFloatAttribute( m_hue, m_attributeStartId + 1, m_connected[1], "Hue" );
	DrawInputFloatAttribute( m_saturation, m_attributeStartId + 2, m_connected[1], "Saturation" );
	DrawInputFloatAttribute( m_value, m_attributeStartId + 3, m_connected[1], "Value" );
	DrawInputFloatAttribute( m_factor, m_attributeStartId + 4, m_connected[1], "Factor", 0, 1 );

	ImGui::Spacing();
	DrawOutputColorAttribute( m_attributeStartId + 5 );

	ImNodes::EndNode();
}

void CHueSaturationValue::Attach( int endAttr )
{
	if ( m_attributeStartId <= endAttr && m_attributeStartId + 6 > endAttr )
		m_connected[endAttr - m_attributeStartId] = true;
}

void CHueSaturationValue::Detach( int endAttr )
{
	if ( m_attributeStartId <= endAttr && m_attributeStartId + 6 > endAttr )
		m_connected[endAttr - m_attributeStartId] = false;
}

void CHueSaturationValue::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//todo add include
	if ( attributeType == EAttributeType::Float )
		output << "( ";
	output << "HueSatVal(";
	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_hue );
	output << ", ";
	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_saturation );
	output << ", ";
	if ( m_connected[2] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_value );
	output << ", ";
	if ( m_connected[3] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
	else
		output << std::to_string( m_factor );
	output << ", ";
	if ( m_connected[4] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
	else
		output << "vec3( " << std::to_string( m_color[0] ) << ", " << std::to_string( m_color[1] ) << ", " << std::to_string( m_color[2] ) << ")";

	output << ");";
	if ( attributeType == EAttributeType::Float )
		output << " ).x";
}

bool CHueSaturationValue::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId <= endAttr && m_attributeStartId + 6 > endAttr )
		return m_connected[endAttr - m_attributeStartId];
	return false;
}

int CHueSaturationValue::GetId() const
{
	return m_id;
}

std::vector<int> CHueSaturationValue::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3, m_attributeStartId + 4 };
}

CInvert::CInvert( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connected{}
	, m_color{}
	, m_factor{ 0 }
{
	attributeStartId += 3;
}

CInvert::~CInvert()
= default;

void CInvert::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "Invert" );
	ImNodes::EndNodeTitleBar();

	DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
	DrawInputFloatAttribute( m_factor, m_attributeStartId + 1, m_connected[1], "Factor" );

	ImGui::Spacing();
	DrawOutputColorAttribute( m_attributeStartId + 2 );

	ImNodes::EndNode();
}

void CInvert::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
}

void CInvert::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
}

void CInvert::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//todo add include
	if ( attributeType == EAttributeType::Float )
		output << "( ";
	output << "Invert( ";

	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_color[0] ) << ", " << std::to_string( m_color[1] ) << ", " << std::to_string( m_color[2] ) << ")";
	output << ", ";
	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << std::to_string( m_factor );
	output << " )";
	if ( attributeType == EAttributeType::Float )
		output << ").x ";
}

bool CInvert::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connected[1];
	return false;
}

int CInvert::GetId() const
{
	return m_id;
}

std::vector<int> CInvert::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1 };
}

CMix::CMix( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_type{ EMode::Multiply }
	, m_connected{}
	, m_color1{}
	, m_color2{}
	, m_factor{ 0 }
{
	attributeStartId += 4;
}

CMix::~CMix()
= default;

void CMix::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( ToString( m_type ) );
	ImNodes::EndNodeTitleBar();
	{
		if ( ImGui::BeginCombo( "##Mode", ToString( m_type ) ) )
		{
			for ( int n = 0; n < ( int ) EMode::MaxIndex; n++ )
			{
				const bool isSelected = ( ( int ) m_type == n );
				if ( ImGui::Selectable( ToString( EMode( n ) ), isSelected ) )
					m_type = EMode( n );

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if ( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	DrawInputColorAttribute( m_color1, m_attributeStartId, m_connected[0], "Color" );
	DrawInputColorAttribute( m_color1, m_attributeStartId + 1, m_connected[1], "Color" );
	DrawInputFloatAttribute( m_factor, m_attributeStartId + 2, m_connected[2], "Factor", 0, 1 );

	ImGui::Spacing();
	DrawOutputColorAttribute( m_attributeStartId + 3 );

	ImNodes::EndNode();
}

void CMix::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = true;
}

void CMix::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = false;
}

void CMix::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	//todo add include
	if ( attributeType == EAttributeType::Float )
		output << "( ";
	switch ( m_type )
	{
		case EMode::Add:
			output << "Add( ";
			break;
		case EMode::Subtract:
			output << "Subtract( ";
			break;
		case EMode::Multiply:
			output << "Multiply( ";
			break;
		case EMode::Screen:
			output << "Screen( ";
			break;
		case EMode::Divide:
			output << "Divide( ";
			break;
		case EMode::Difference:
			output << "Difference( ";
			break;
		case EMode::Darken:
			output << "Darken( ";
			break;
		case EMode::Lighten:
			output << "Lighten( ";
			break;
		case EMode::Overlay:
			output << "Overlay( ";
			break;
		case EMode::ColorDodge:
			output << "ColorDodge( ";
			break;
		case EMode::ColorBurn:
			output << "ColorBurn( ";
			break;
		case EMode::Hue:
			output << "Hue( ";
			break;
		case EMode::Saturation:
			output << "Saturation( ";
			break;
		case EMode::Value:
			output << "Value( ";
			break;
		case EMode::ColorMix:
			output << "ColorMix( ";
			break;
		case EMode::SoftLight:
			output << "SoftLight( ";
			break;
		case EMode::LinearLight:
			output << "LinearLight( ";
			break;
		default:
			break;
	}
	if ( m_connected[0] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_color1[0] ) << ", " << std::to_string( m_color1[1] ) << ", " << std::to_string( m_color1[2] ) << ")";

	output << ", ";
	if ( m_connected[1] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << "vec3( " << std::to_string( m_color2[0] ) << ", " << std::to_string( m_color2[1] ) << ", " << std::to_string( m_color2[2] ) << ")";

	output << ", ";
	if ( m_connected[2] )
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
	else
		output << std::to_string( m_factor );

	output << " )";
	if ( attributeType == EAttributeType::Float )
		output << " ).x";
}

bool CMix::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connected[1];
	if ( m_attributeStartId + 2 == endAttr )
		return m_connected[2];
	return false;
}

int CMix::GetId() const
{
	return m_id;
}

std::vector<int> CMix::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}

const char* CMix::ToString( EMode mode )
{
	switch ( mode )
	{
		case EMode::Add:
			return "Add";
		case EMode::Subtract:
			return "Subtract";
		case EMode::Multiply:
			return "Multiply";
		case EMode::Screen:
			return "Screen";
		case EMode::Divide:
			return "Divide";
		case EMode::Difference:
			return "Difference";
		case EMode::Darken:
			return "Darken";
		case EMode::Lighten:
			return "Lighten";
		case EMode::Overlay:
			return "Overlay";
		case EMode::ColorDodge:
			return "ColorDodge";
		case EMode::ColorBurn:
			return "ColorBurn";
		case EMode::Hue:
			return "Hue";
		case EMode::Saturation:
			return "Saturation";
		case EMode::Value:
			return "Value";
		case EMode::ColorMix:
			return "Color Mix";
		case EMode::SoftLight:
			return "Soft Light";
		case EMode::LinearLight:
			return "Linear Light";
		default:
			return "";
	}
}
