#include "pch.h"
#include "ShaderGraph.h"

#include <fstream>
#include <imgui.h>
#include <imnodes.h>
#include <SDL_scancode.h>

#include "Nodes/Bump.h"
#include "Nodes/Clamp.h"
#include "Nodes/ColorNodes.h"
#include "Nodes/Displacement.h"
#include "Nodes/FragmentOutput.h"
#include "Nodes/Mapping.h"
#include "Nodes/MathNode.h"
#include "Nodes/NormalMap.h"
#include "Nodes/RGBtoBW.h"
#include "Nodes/VectorMath.h"
#include "Nodes/VertexOutput.h"

BalEditor::CShaderGraph::CShaderGraph()
	:m_isVisible{ false }
{
	AddNode( EUiNodeType::VertexOutput, glm::vec2{ 420, 128 } );
	AddNode( EUiNodeType::FragmentOutput, glm::vec2{ 420, 240 } );
}

BalEditor::CShaderGraph::~CShaderGraph()
{
	for ( const auto& element : m_nodes )
	{
		delete element;
	}
	m_nodes.clear();
	m_links.clear();
}

void BalEditor::CShaderGraph::Draw()
{
	if( m_isVisible && ImGui::Begin( "Shader Graph", &m_isVisible, ImGuiWindowFlags_MenuBar ))
	{
		if ( ImGui::BeginMenuBar() )
		{
			if ( ImGui::Button( "Save " ) )
				Evaluate();
			ImGui::EndMenuBar();
		}

		ImNodes::BeginNodeEditor();
		if ( ImGui::BeginPopupContextWindow( "Add Node", 1, false ) )
		{
			ImGui::Text( "Add Node" );
			for ( int n = 2; n < ( int ) EUiNodeType::MaxIndex; n++ )
			{
				if ( ImGui::MenuItem( ToString( EUiNodeType( n ) ) ) )
				{
					const ImVec2 mousePos = ImGui::GetMousePos();
					AddNode( ( EUiNodeType )n, glm::vec2{mousePos.x, mousePos.y});
				}
			}
			ImGui::EndPopup();
		}

		for ( INode* node : m_nodes )
		{
			node->Draw();
		}

		for ( const SLink& link : m_links )
		{
			ImNodes::Link( link.id, link.startAttr, link.endAttr );
		}

		ImNodes::EndNodeEditor();

		{
			SLink link;
			if ( ImNodes::IsLinkCreated( &link.startNodeId, &link.startAttr, &link.endNodeId, &link.endAttr ) )
			{
				int nodeId{ link.endNodeId };
				const auto it = std::ranges::find_if( m_nodes, [&nodeId]( INode* node )
				{
					return node->GetId() == nodeId;
				} );
				if ( it != m_nodes.end() && ( *it )->HasFreeAttachment( link.endAttr ) )
				{
					( *it )->Attach( link.endAttr );
					link.id = m_currentId++;
					m_links.push_back( link );
				}
			}
		}

		{
			int linkId;
			if ( ImNodes::IsLinkDestroyed( &linkId ) )
			{
				const auto iterator = std::ranges::find_if( m_links
															, [linkId]( const SLink& link ) -> bool
				{
					return link.id == linkId;
				} );
				assert( iterator != m_links.end() );
				int nodeId{ iterator->endNodeId };
				const auto it = std::ranges::find_if( m_nodes, [&nodeId]( INode* node )
				{
					return node->GetId() == nodeId;
				} );
				( *it )->Detach( iterator->endAttr );
				m_links.erase( iterator );
			}
		}
		ImGui::End();
	}
}

void BalEditor::CShaderGraph::ShowWindow()
{
	m_isVisible = true;
	ImGui::SetWindowFocus( "Shader Graph" );
}

std::vector<BalEditor::CShaderGraph::SLink> BalEditor::CShaderGraph::GetNeighbors( const int currentNode )
{
	std::vector<SLink> links;
	std::vector<SLink>::iterator it = m_links.begin();

	while ( ( it = std::find_if( it, m_links.end(), [&currentNode]( const SLink& link )
	{
		return link.endNodeId == currentNode;
	} ) ) != m_links.end() )
	{
		links.push_back( *it );
		++it;
	}
	std::reverse( links.begin(), links.end() );
	return links;
}

void BalEditor::CShaderGraph::Evaluate()
{
	std::vector<INode*> postOrder;
	std::vector<INode*> stack;

	stack.push_back( m_nodes[0] );

	//todo: save nodes that are used multiple times to separate variable
	while ( !stack.empty() )
	{
		INode* pCurrentNode = stack.back();
		stack.pop_back();

		postOrder.push_back( pCurrentNode );

		std::vector connected{ GetNeighbors( pCurrentNode->GetId() ) };
		for ( const SLink& neighbor : connected )
		{
			int nodeId = neighbor.startNodeId;
			auto nodeIterator = std::ranges::find_if( m_nodes, [nodeId]( INode* pNode )
			{
				return pNode->GetId() == nodeId;
			} );
			stack.push_back( *nodeIterator );
		}
	}

	std::ofstream verextFile{ "shader.vert", std::ios::out | std::ios::binary };
	std::vector<INode*>::iterator nextNode{};
	if ( postOrder.empty() )
		nextNode = postOrder.end();
	else
		nextNode = postOrder.begin() + 1;

	m_nodes[0]->Evaluate( nextNode, postOrder.end(), verextFile );

	//add fragment shader
	postOrder.clear();
	stack.clear();

	stack.push_back( m_nodes[1] );

	//todo: save nodes that are used multiple times to separate variable
	while ( !stack.empty() )
	{
		INode* pCurrentNode = stack.back();
		stack.pop_back();

		postOrder.push_back( pCurrentNode );

		std::vector connected{ GetNeighbors( pCurrentNode->GetId() ) };
		for ( const SLink& neighbor : connected )
		{
			int nodeId = neighbor.startNodeId;
			auto nodeIterator = std::ranges::find_if( m_nodes, [nodeId]( INode* pNode )
			{
				return pNode->GetId() == nodeId;
			} );
			stack.push_back( *nodeIterator );
		}
	}

	std::ofstream fragmentFile{ "shader.frag", std::ios::out | std::ios::binary };
	if ( postOrder.empty() )
		nextNode = postOrder.end();
	else
		nextNode = postOrder.begin() + 1;

	m_nodes[1]->Evaluate( nextNode, postOrder.end(), fragmentFile );

	//todo: save nodes graph
}

void BalEditor::CShaderGraph::AddNode( EUiNodeType type, const glm::vec2& position )
{
	INode* pNode{ nullptr };
	const int id = m_currentId++;
	switch ( type )
	{
		case EUiNodeType::VertexOutput:
			pNode = new CVertexOutputNode{ id, m_currentAttributeId };
			break;
		case EUiNodeType::FragmentOutput:
			pNode = new CFragmentOutputNode{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Math:
			pNode = new CMathNode{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Clamp:
			pNode = new CClamp{ id, m_currentAttributeId };
			break;
		case EUiNodeType::BrightContrast:
			pNode = new CBrightContrast{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Gamma:
			pNode = new CGamma{ id, m_currentAttributeId };
			break;
		case EUiNodeType::HueSaturationValue:
			pNode = new CHueSaturationValue{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Invert:
			pNode = new CInvert{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Mix:
			pNode = new CMix{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Bump:
			pNode = new CBump{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Displacement:
			pNode = new CDisplacement{ id, m_currentAttributeId };
			break;
		case EUiNodeType::Mapping:
			pNode = new CMapping{ id, m_currentAttributeId };
			break;
		case EUiNodeType::NormalMap:
			pNode = new CNormalMap{ id, m_currentAttributeId };
			break;
		case EUiNodeType::RGBtoBW:
			pNode = new CRGBtoBW{ id, m_currentAttributeId };
			break;
		case EUiNodeType::VectorMath:
			pNode = new CVectorMath{ id, m_currentAttributeId };
			break;
		default:;
	}
	ImNodes::SetNodeGridSpacePos( id, { position.x ,position.y } );
	m_nodes.push_back( pNode );
}

const char* BalEditor::CShaderGraph::ToString( EUiNodeType type )
{
	switch ( type )
	{
		case EUiNodeType::VertexOutput:
			return "Vertex Output";
		case EUiNodeType::FragmentOutput:
			return "Fragment Output";
		case EUiNodeType::BrightContrast:
			return "BrightContrast";
		case EUiNodeType::Gamma:
			return "Gamma";
		case EUiNodeType::HueSaturationValue:
			return "Hue Saturation Value";
		case EUiNodeType::Invert:
			return "Invert";
		case EUiNodeType::Mix:
			return "Mix";
		case EUiNodeType::Bump:
			return "Bump";
		case EUiNodeType::Displacement:
			return "Displacement";
		case EUiNodeType::Mapping:
			return "Mapping";
		case EUiNodeType::NormalMap:
			return "NormalMap";
		case EUiNodeType::Clamp:
			return "Clamp";
		case EUiNodeType::Math:
			return "Math";
		case EUiNodeType::RGBtoBW:
			return "RGB To BW";
		case EUiNodeType::VectorMath:
			return "Vector Math";
	}
	return "nullptr";
}
