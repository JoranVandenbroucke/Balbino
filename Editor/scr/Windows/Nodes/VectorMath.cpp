#include "pch.h"
#include "VectorMath.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CVectorMath::CVectorMath( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_mode{}
	, m_connected{}
	, m_vectorA{}
	, m_vectorB{}
	, m_vectorC{}
	, m_fac{ 0 }
{
	attributeStartId += 4;
}

CVectorMath::~CVectorMath()
{
}

void CVectorMath::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( ToString( m_mode ) );
	ImNodes::EndNodeTitleBar();
	{
		if ( ImGui::BeginCombo( "##Mode", ToString( m_mode ) ) )
		{
			for ( int n = 0; n < ( int ) EMode::MaxIndex; n++ )
			{
				const bool isSelected = ( ( int ) m_mode == n );
				if ( ImGui::Selectable( ToString( EMode( n ) ), isSelected ) )
					m_mode = EMode( n );

				if ( n == 10 || n == 18 || n == 27 || n == 37 )
					ImGui::Separator();

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if ( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	DrawInputVectorAttribute( m_vectorA, m_attributeStartId, m_connected[0] );
	switch ( m_mode )
	{
		case EMode::Add:
		case EMode::Subtract:
		case EMode::Multiply:
		case EMode::Divide:
		case EMode::CrossProduct:
		case EMode::Project:
		case EMode::Reflect:
		case EMode::DotProduct:
		case EMode::Distance:
		case EMode::Snap:
		case EMode::Modulo:
		case EMode::Minimum:
		case EMode::Maximum:
			DrawInputVectorAttribute( m_vectorB, m_attributeStartId + 1, m_connected[1] );
			break;
		case EMode::MultiplyAdd:
		case EMode::Faceforward:
		case EMode::Wrap:
			DrawInputVectorAttribute( m_vectorB, m_attributeStartId + 1, m_connected[1] );
			DrawInputVectorAttribute( m_vectorC, m_attributeStartId + 2, m_connected[2] );
			break;
		case EMode::Scale:
			DrawInputFloatAttribute( m_fac, m_attributeStartId + 1, m_connected[2] );
			break;
		case EMode::Refract:
			DrawInputVectorAttribute( m_vectorB, m_attributeStartId + 1, m_connected[1] );
			DrawInputFloatAttribute( m_fac, m_attributeStartId + 2, m_connected[2] );
			break;
		default:;
	}

	ImGui::Spacing();
	DrawOutputVectorAttribute( m_attributeStartId + 3 );

	ImNodes::EndNode();
}

void CVectorMath::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = true;
}

void CVectorMath::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	else if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
	else if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = false;
}

void CVectorMath::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
			if ( attributeType == EAttributeType::Float )
				output << "( ";
	switch ( m_mode )
	{
		case EMode::Add:
		{
			output << "Add( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Subtract:
		{
			output << "Subtract( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Multiply:
		{
			output << "Multiply( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Divide:
		{
			output << "Divide( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::MultiplyAdd:
		{
			output << "MultiplyAdd( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << ", ";
			if ( m_connected[2] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorC[0] ) << ", " << std::to_string( m_vectorC[1] ) << ", " << std::to_string( m_vectorC[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::CrossProduct:
		{
			output << "CrossProduct( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Project:
		{
			output << "Project( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Reflect:
		{
			output << "Reflect( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Refract:
		{
			output << "Refract( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << ", ";
			if ( m_connected[2] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_fac );
			output << " )";
			break;
		}
		case EMode::Faceforward:
		{
			output << "Faceforward( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << ", ";
			if ( m_connected[2] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorC[0] ) << ", " << std::to_string( m_vectorC[1] ) << ", " << std::to_string( m_vectorC[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::DotProduct:
		{
			output << "EMode( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Distance:
		{
			output << "Distance( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Length:
		{
			output << "Length( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Scale:
		{
			output << "Scale( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[2] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
			else
				output << std::to_string( m_fac );
			output << " )";
			break;
		}
		case EMode::Normalize:
		{
			output << "Normalize( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Wrap:
		{
			output << "Wrap( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << ", ";
			if ( m_connected[2] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorC[0] ) << ", " << std::to_string( m_vectorC[1] ) << ", " << std::to_string( m_vectorC[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Snap:
		{
			output << "Snap( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Floor:
		{
			output << "Floor( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Ceil:
		{
			output << "Ceil( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Modulo:
		{
			output << "Modulo( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Fraction:
		{
			output << "Fraction( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Absolute:
		{
			output << "Absolute( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Minimum:
		{
			output << "Minimum( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Maximum:
		{
			output << "Maximum( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << ", ";
			if ( m_connected[1] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorB[0] ) << ", " << std::to_string( m_vectorB[1] ) << ", " << std::to_string( m_vectorB[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Sine:
		{
			output << "Sine( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Cosine:
		{
			output << "Cosine( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
		case EMode::Tangent:
		{
			output << "Tangent( ";
			if ( m_connected[0] )
				( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
			else
				output << "vec3( " << std::to_string( m_vectorA[0] ) << ", " << std::to_string( m_vectorA[1] ) << ", " << std::to_string( m_vectorA[2] ) << " )";
			output << " )";
			break;
		}
	}
			if ( attributeType == EAttributeType::Float )
				output << " ).x";
}

bool CVectorMath::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return m_connected[1];
	if ( m_attributeStartId + 2 == endAttr )
		return m_connected[2];
	return false;
}

int CVectorMath::GetId() const
{
	return m_id;
}

std::vector<int> CVectorMath::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}

const char* CVectorMath::ToString( EMode type )
{
	switch ( type )
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
			return "MultiplyAdd";
		case EMode::CrossProduct:
			return "CrossProduct";
		case EMode::Project:
			return "Project";
		case EMode::Reflect:
			return "Reflect";
		case EMode::Refract:
			return "Refract";
		case EMode::Faceforward:
			return "Face Forward";
		case EMode::DotProduct:
			return "DotProduct";
		case EMode::Distance:
			return "Distance";
		case EMode::Length:
			return "Length";
		case EMode::Scale:
			return "Scale";
		case EMode::Normalize:
			return "Normalize";
		case EMode::Wrap:
			return "Wrap";
		case EMode::Snap:
			return "Snap";
		case EMode::Floor:
			return "Floor";
		case EMode::Ceil:
			return "Ceil";
		case EMode::Modulo:
			return "Modulo";
		case EMode::Fraction:
			return "Fraction";
		case EMode::Absolute:
			return "Absolute";
		case EMode::Minimum:
			return "Minimum";
		case EMode::Maximum:
			return "Maximum";
		case EMode::Sine:
			return "Sine";
		case EMode::Cosine:
			return "Cosine";
		case EMode::Tangent:
			return "Tangent";
	}
	return "";
}
