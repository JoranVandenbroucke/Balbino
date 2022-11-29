#include "Mapping.h"

#include <string>
#include <format>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CMapping::CMapping( const int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connected{}
	, m_vector{}
	, m_position{}
	, m_rotation{}
	, m_scale{}
	, m_type{Point}
{
	attributeStartId += 5;
}

CMapping::~CMapping()
= default;

void CMapping::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::Text( "%s", ToString( m_type ) );
	ImNodes::EndNodeTitleBar();
	{
		if ( ImGui::BeginCombo( "##Mode", ToString( m_type ) ) )
		{
			for ( int n = 0; n < static_cast<int>( EMode::MaxIndex ); n++ )
			{
				const bool isSelected = ( static_cast<int>( m_type ) == n );
				if ( ImGui::Selectable( ToString( static_cast<EMode>( n ) ), isSelected ) )
					m_type = static_cast<EMode>( n );

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if ( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	DrawInputVectorAttribute( m_vector, m_attributeStartId, false, "Vector" );
	DrawInputVectorAttribute( m_position, m_attributeStartId + 1, m_connected[1], "Color" );
	DrawInputVectorAttribute( m_rotation, m_attributeStartId + 2, m_connected[2], "Color" );
	DrawInputVectorAttribute( m_scale, m_attributeStartId + 3, m_connected[3], "Color" );

	ImGui::Spacing();
	DrawOutputVectorAttribute( m_attributeStartId + 4 );

	ImNodes::EndNode();
}

void CMapping::Attach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = true;
	if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = true;
	if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = true;
	if ( m_attributeStartId + 3 == endAttr )
		m_connected[3] = true;
}

void CMapping::Detach( int endAttr )
{
	if ( m_attributeStartId == endAttr )
		m_connected[0] = false;
	if ( m_attributeStartId + 1 == endAttr )
		m_connected[1] = false;
	if ( m_attributeStartId + 2 == endAttr )
		m_connected[2] = false;
	if ( m_attributeStartId + 3 == endAttr )
		m_connected[3] = false;
}

std::string CMapping::Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
	( void ) begin;
	( void ) bindings;
	( void ) attributeType;
    includes.insert("mapping.glsl");
    std::string shader;
	switch ( m_type )
	{
		case Point:
            shader = std::format( "MappingPoint({},{},{},{})"
                    , m_connected[0] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_vector[0] , m_vector[1] , m_vector[2] )
                    , m_connected[1] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_position[0] , m_position[1] , m_position[2] )
                    , m_connected[2] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_rotation[0] , m_rotation[1] , m_rotation[2] )
                    , m_connected[3] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_scale[0] , m_scale[1] , m_scale[2] )
            );
            break;
        case Texture:
            shader = std::format( "MappingTexture({},{},{},{})"
                    , m_connected[0] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_vector[0] , m_vector[1] , m_vector[2] )
                    , m_connected[1] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_position[0] , m_position[1] , m_position[2] )
                    , m_connected[2] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_rotation[0] , m_rotation[1] , m_rotation[2] )
                    , m_connected[3] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_scale[0] , m_scale[1] , m_scale[2] )
            );
            break;
		case Vector:
            shader = std::format( "MappingVector({},{},{},{})"
                    , m_connected[0] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_vector[0] , m_vector[1] , m_vector[2] )
                    , m_connected[1] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_position[0] , m_position[1] , m_position[2] )
                    , m_connected[2] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_rotation[0] , m_rotation[1] , m_rotation[2] )
                    , m_connected[3] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_scale[0] , m_scale[1] , m_scale[2] )
            );

			break;
		case Normal:
            shader = std::format( "MappingNormal({},{},{},{})"
                    , m_connected[0] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_vector[0] , m_vector[1] , m_vector[2] )
                    , m_connected[1] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_position[0] , m_position[1] , m_position[2] )
                    , m_connected[2] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_rotation[0] , m_rotation[1] , m_rotation[2] )
                    , m_connected[3] ? ( *( begin++ ) )->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : std::format("vec3({},{},{})" , m_scale[0] , m_scale[1] , m_scale[2] )
            );
			break;
        case MaxIndex:
            break;
    }

	if ( attributeType == EAttributeType::Float )
        shader += ".x";
    return shader;
}

bool CMapping::HasFreeAttachment( int endAttr ) const
{
	if ( m_attributeStartId == endAttr )
		return !m_connected[0];
	if ( m_attributeStartId + 1 == endAttr )
		return !m_connected[1];
	if ( m_attributeStartId + 2 == endAttr )
		return !m_connected[2];
	if ( m_attributeStartId + 3 == endAttr )
		return !m_connected[3];
	return false;
}

int CMapping::GetId() const
{
	return m_id;
}

std::vector<int> CMapping::GetInputs() const
{
	return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3 };
}

const char* CMapping::ToString( EMode type )
{
	switch ( type )
	{
		case Point:
			return "Point";
		case Texture:
			return "texture";
		case Vector:
			return "Vector";
		case Normal:
			return "Normal";
        case MaxIndex:
            break;
    }
	return "";
}
