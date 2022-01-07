#include "pch.h"
#include "MathNode.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

const char* CMathNode::ToString( const EMode e )
{
	switch ( e )
	{
		case EMode::Add:
			return "Add";
		case EMode::Subtract:
			return "Subtract";
		case EMode::Multiply:
			return "Multiply";
		case EMode::Divide:
			return "Divide";
		case EMode::MultiplyAdd:
			return "Multiply Add";
		case EMode::Power:
			return "Power";
		case EMode::Logarithm:
			return "Logarithm";
		case EMode::SquareRoot:
			return "Square Root";
		case EMode::InverseSquareRoot:
			return "Inverse Square Root";
		case EMode::Absolute:
			return "Absolute";
		case EMode::Exponent:
			return "Exponent";
		case EMode::Minimum:
			return "Minimum";
		case EMode::Maximum:
			return "Maximum";
		case EMode::LessThan:
			return "Less Than";
		case EMode::GreaterThan:
			return "Greater Than";
		case EMode::Sign:
			return "Sign";
		case EMode::Compare:
			return "Compare";
		case EMode::SmoothMinimum:
			return "Smooth Minimum";
		case EMode::SmoothMaximum:
			return "Smooth Max";
		case EMode::Round:
			return "Round";
		case EMode::Floor:
			return "Floor";
		case EMode::Ceil:
			return "Ceil";
		case EMode::Truncate:
			return "Truncate";
		case EMode::Fraction:
			return "Fraction";
		case EMode::Modulo:
			return "Modulo";
		case EMode::Wrap:
			return "Wrap";
		case EMode::Snap:
			return "Snap";
		case EMode::PingPong:
			return "Ping - Pong";
		case EMode::Sine:
			return "Sin";
		case EMode::Cosine:
			return "Cosine";
		case EMode::Tangent:
			return "Tangent";
		case EMode::Arcsine:
			return "Arcsine";
		case EMode::Arccosine:
			return "Arccosine";
		case EMode::Arctangent:
			return "Arctangent";
		case EMode::Arctan2:
			return "Arctangent2";
		case EMode::HyperbolicSine:
			return "Hyperbolic Sin";
		case EMode::HyperbolicCosine:
			return "Hyperbolic Cosine";
		case EMode::HyperbolicTangent:
			return "Hyperbolic Tangent";
		case EMode::ToRadians:
			return "To Radians";
		case EMode::ToDegrees:
			return "To Degree";
		case EMode::MaxIndex:
			return "unknown";
	}
	return nullptr;
}

CMathNode::CMathNode( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_currentMode{}
	, m_clamp{ false }
	, m_a{ 0 }
	, m_b{ 0 }
	, m_c{ 0 }
{
	attributeStartId += 4;
}

void CMathNode::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( ToString( m_currentMode ) );
	ImNodes::EndNodeTitleBar();
	{
		if ( ImGui::BeginCombo( "##Mode", ToString( m_currentMode ) ) )
		{
			for ( int n = 0; n < ( int ) EMode::MaxIndex; n++ )
			{
				const bool isSelected = ( ( int ) m_currentMode == n );
				if ( ImGui::Selectable( ToString( EMode( n ) ), isSelected ) )
					m_currentMode = EMode( n );

				if ( n == 10 || n == 18 || n == 27 || n == 37 )
					ImGui::Separator();

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if ( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Checkbox( "Clam", &m_clamp );
	DrawInputFloatAttribute( m_a, m_attributeStartId, m_connectedA );
	switch ( m_currentMode )
	{
		//2
		case EMode::Add:
		case EMode::Subtract:
		case EMode::Multiply:
		case EMode::Divide:
		case EMode::Power:
		case EMode::Logarithm:
		case EMode::Exponent:
		case EMode::Minimum:
		case EMode::Maximum:
		case EMode::LessThan:
		case EMode::GreaterThan:
		case EMode::Fraction:
		case EMode::Modulo:
		case EMode::Snap:
		case EMode::PingPong:
		case EMode::Arctan2:
			{
				DrawInputFloatAttribute( m_b, m_attributeStartId + 1, m_connectedB );
				break;
			}
		//3
		case EMode::MultiplyAdd:
		case EMode::SmoothMinimum:
		case EMode::SmoothMaximum:
		case EMode::Floor:
		case EMode::Ceil:
		case EMode::Truncate:
			{
				DrawInputFloatAttribute( m_b, m_attributeStartId + 1, m_connectedB );
				DrawInputFloatAttribute( m_c, m_attributeStartId + 2, m_connectedC );
				break;
			}
		default:
			break;
	}

	ImGui::Spacing();
	DrawOutputFloatAttribute( m_attributeStartId + 3 );

	ImNodes::EndNode();
}

void CMathNode::Attach( const int endAttr )
{
	if ( endAttr == m_attributeStartId )
	{
		m_connectedA = true;
	}
	else if ( endAttr == m_attributeStartId + 1 )
	{
		m_connectedB = true;
	}
	else if ( endAttr == m_attributeStartId + 2 )
	{
		m_connectedC = true;
	}
}

void CMathNode::Detach( const int endAttr )
{
	if ( endAttr == m_attributeStartId )
	{
		m_connectedA = false;
	}
	else if ( endAttr == m_attributeStartId + 1 )
	{
		m_connectedB = false;
	}
	else if ( endAttr == m_attributeStartId + 2 )
	{
		m_connectedC = false;
	}
}

void CMathNode::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	if ( attributeType != EAttributeType::Float )
		output << "vec3( ";
	if ( m_clamp )
		output << "clamp( ";
	switch ( m_currentMode )
	{
		case EMode::Add:
			output << "MathAdd(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Subtract:
			output << "MathSubtract(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Multiply:
			output << "MathMultiply(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Divide:
			output << "MathDivide(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::MultiplyAdd:
			output << "MathMultiply Add(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			output << ", ";
			if ( m_connectedC )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_c );
			break;
		case EMode::Power:
			output << "MathPower(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Logarithm:
			output << "MathLogarithm(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::SquareRoot:
			output << "MathSquare Root(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::InverseSquareRoot:
			output << "MathInverse Square Root(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Absolute:
			output << "MathAbsolute(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Exponent:
			output << "MathExponent(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Minimum:
			output << "MathMinimum(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Maximum:
			output << "MathMaximum(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::LessThan:
			output << "MathLess Than(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::GreaterThan:
			output << "MathGreater Than(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Sign:
			output << "MathSign(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Compare:
			output << "MathCompare(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			output << ", ";
			if ( m_connectedC )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_c );
			break;
		case EMode::SmoothMinimum:
			output << "MathSmooth Minimum(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			output << ", ";
			if ( m_connectedC )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_c );
			break;
		case EMode::SmoothMaximum:
			output << "MathSmooth Max(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			output << ", ";
			if ( m_connectedC )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_c );
			break;
		case EMode::Round:
			output << "MathRound(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Floor:
			output << "MathFloor(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Ceil:
			output << "MathCeil(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Truncate:
			output << "MathTruncate(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Fraction:
			output << "MathFraction(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Modulo:
			output << "MathModulo(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Wrap:
			output << "MathWrap(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			output << ", ";
			if ( m_connectedC )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_c );
			break;
		case EMode::Snap:
			output << "MathSnap(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::PingPong:
			output << "MathPing - Pong(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::Sine:
			output << "MathSin(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Cosine:
			output << "MathCosine(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Tangent:
			output << "MathTangent(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Arcsine:
			output << "MathArcsine(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Arccosine:
			output << "MathArccosine(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Arctangent:
			output << "MathArctangent(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::Arctan2:
			output << "MathArctangent2(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			output << ", ";
			if ( m_connectedB )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_b );
			break;
		case EMode::HyperbolicSine:
			output << "MathHyperbolic Sin(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::HyperbolicCosine:
			output << "MathHyperbolic Cosine(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::HyperbolicTangent:
			output << "MathHyperbolic Tangent(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::ToRadians:
			output << "MathTo Radians(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		case EMode::ToDegrees:
			output << "MathTo Degree(";
			if ( m_connectedA )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_a );
			break;
		default: ;
	}
	output << " )";
	if ( m_clamp )
		output << " )";
	if ( attributeType != EAttributeType::Float )
		output << " )";
}

bool CMathNode::HasFreeAttachment( const int endAttr ) const
{
	if ( !m_connectedA && endAttr == m_attributeStartId )
		return true;
	switch ( m_currentMode )
	{
		//2
		case EMode::Add:
		case EMode::Subtract:
		case EMode::Multiply:
		case EMode::Divide:
		case EMode::Power:
		case EMode::Logarithm:
		case EMode::Exponent:
		case EMode::Minimum:
		case EMode::Maximum:
		case EMode::LessThan:
		case EMode::GreaterThan:
		case EMode::Fraction:
		case EMode::Modulo:
		case EMode::Snap:
		case EMode::PingPong:
		case EMode::Arctan2:
			{
				if ( !m_connectedB && endAttr == m_attributeStartId + 1 )
					return true;
				break;
			}
		//3
		case EMode::MultiplyAdd:
		case EMode::SmoothMinimum:
		case EMode::SmoothMaximum:
		case EMode::Floor:
		case EMode::Ceil:
		case EMode::Truncate:
			{
				if ( !m_connectedB && endAttr == m_attributeStartId + 1 )
					return true;
				if ( !m_connectedC && endAttr == m_attributeStartId + 2 )
					return true;
				break;
			}
		default:
			break;
	}
	if ( endAttr == m_attributeStartId + 3 )
		return true;
	return false;
}

int CMathNode::GetId() const
{
	return m_id;
}

std::vector<int> CMathNode::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}
