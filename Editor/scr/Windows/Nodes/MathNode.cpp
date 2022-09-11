
#include "MathNode.h"

#include <string>
#include <format>

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
	ImGui::Text( "%s", ToString( m_currentMode ) );
	ImNodes::EndNodeTitleBar();
	{
		if ( ImGui::BeginCombo( "##Mode", ToString( m_currentMode ) ) )
		{
			for ( int n = 0; n < static_cast<int>( EMode::MaxIndex ); n++ )
			{
				const bool isSelected = ( static_cast<int>( m_currentMode ) == n );
				if ( ImGui::Selectable( ToString( static_cast<EMode>( n ) ), isSelected ) )
					m_currentMode = static_cast<EMode>( n );

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

std::string CMathNode::Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType )
{
	( void ) begin;
	( void ) bindings;
	( void ) attributeType;
    includes.insert("math.glsl");
    std::string shader;

	switch ( m_currentMode )
	{
		case EMode::Add:
            shader = std::format("MathAdd({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    );
			break;
		case EMode::Subtract:
            shader = std::format("MathSubtract({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Multiply:
            shader = std::format("MathMultiply({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Divide:
            shader = std::format("MathDivide({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
            break;
		case EMode::MultiplyAdd:
            shader = std::format("MathMultiplyAdd({},{},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    ,m_connectedC? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_c)
            );
			break;
		case EMode::Power:
            shader = std::format("MathPower({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Logarithm:
            shader = std::format("MathLogarithm({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
            break;
		case EMode::SquareRoot:
            shader = std::format("MathSquareRoot({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );break;
		case EMode::InverseSquareRoot:
            shader = std::format("MathInverseSquareRoot({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Absolute:
            shader = std::format("MathAbsolute({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Exponent:
            shader = std::format("MathExponent({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Minimum:
            shader = std::format("MathMinimum({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Maximum:
            shader = std::format("MathMaximum({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
            break;
		case EMode::LessThan:
            shader = std::format("MathLessThan({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::GreaterThan:
            shader = std::format("MathGreaterThan({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Sign:
            shader = std::format("MathSign({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
            break;
		case EMode::Compare:
            shader = std::format("MathCompare({},{},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    ,m_connectedC? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_c)
            );
            break;
		case EMode::SmoothMinimum:
            shader = std::format("MathSmoothMinimum({},{},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    ,m_connectedC? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_c)
            );
            break;
		case EMode::SmoothMaximum:
            shader = std::format("MathSmoothMax({},{},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    ,m_connectedC? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_c)
            );
			break;
		case EMode::Round:
            shader = std::format("MathRound({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Floor:
            shader = std::format("MathFloor({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Ceil:
            shader = std::format("MathCeil({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Truncate:
            shader = std::format("MathTruncate({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Fraction:
            shader = std::format("MathFraction({},{},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    ,m_connectedC? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_c)
            );
            shader = std::format("MathFraction({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Modulo:
            shader = std::format("MathModulo({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Wrap:
            shader = std::format("MathWrap({},{},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
                    ,m_connectedC? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_c)
            );
			break;
		case EMode::Snap:
            shader = std::format("MathSnap({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::PingPong:
            shader = std::format("MathPingPong({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::Sine:
            shader = std::format("MathSin({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
            break;
		case EMode::Cosine:
            shader = std::format("MathCosine({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Tangent:
            shader = std::format("MathTangent({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Arcsine:
            shader = std::format("MathArcsine({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Arccosine:
            shader = std::format("MathTangent({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Arctangent:
            shader = std::format("MathArctangent({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::Arctan2:
            shader = std::format("MathArctangent2({},{})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
                    ,m_connectedB? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_b)
            );
			break;
		case EMode::HyperbolicSine:
            shader = std::format("MathHyperbolicSin({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::HyperbolicCosine:
            shader = std::format("MathHyperbolicCosine({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::HyperbolicTangent:
            shader = std::format("MathHyperbolicTangent({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::ToRadians:
            shader = std::format("MathToRadians({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		case EMode::ToDegrees:
            shader = std::format("MathToDegree({})"
                    ,m_connectedA? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Float ) : std::to_string(m_a)
            );
			break;
		default: ;
	}
    if ( m_clamp )
        shader = std::format("clamp({})", shader);
	if ( attributeType != EAttributeType::Float )
        shader = std::format("vec3({})", shader);
    return shader;
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
