//
// Created by joran on 25/02/2023.
//

#pragma once
#include "Nodes/ShaderNode.h"
#include "Serializer.h"
#include "shaderc/shaderc.hpp"
#include <list>
#include <set>

class CShaderFileIncluder final : public shaderc::CompileOptions::IncluderInterface
{
public:
    CShaderFileIncluder()           = default;
    ~CShaderFileIncluder() override = default;

    // Resolves a requested source file of a given type from a requesting
    // source into a shaderc_include_result whose contents will remain valid
    // until it's released.
    shaderc_include_result* GetInclude( const char* requestedSource, shaderc_include_type type, const char* requestingSource, std::size_t includeDepth ) override;
    // Releases an include result.
    void ReleaseInclude( shaderc_include_result* includeResult ) override;

    // Returns a reference to the member storing the set of included files.
    [[nodiscard]] const std::unordered_set<std::string, StringHash, std::equal_to<>>& FilePathTrace() const;

private:
    // The set of full paths of included files.
    std::unordered_set<std::string, StringHash, std::equal_to<>> m_includedFiles;
};
class CShaderGraph
{
public:
    CShaderGraph()                                 = default;
    ~CShaderGraph()                                = default;
    CShaderGraph( const CShaderGraph& )            = delete;
    CShaderGraph( CShaderGraph&& )                 = delete;
    CShaderGraph& operator=( const CShaderGraph& ) = delete;
    CShaderGraph& operator=( CShaderGraph&& )      = delete;

    void Initialize();
    void Cleanup();

    CShaderNode* Add( CShaderNode* node );
    void Remove( CShaderNode* node );

    template<typename T, typename... Args>
    T* CreateNode( Args&&... args )
    {
        return new T( m_nodeId, args... );
    }
    template<typename T>
    constexpr void DeleteNode( T* node )
    {
        delete node;
    }

    void Connect( int fromNode, int fromAttr, int toNode, int toAttr ) noexcept;
    void Connect( CShaderOutput* from, CShaderInput* to ) noexcept;
    void Disconnect( int fromNode, int fromAttr, int toNode, int toAttr ) noexcept;
    void Disconnect( CShaderOutput* from ) const noexcept;
    void Disconnect( CShaderInput* to ) const noexcept;

    CShaderNode* GetNode( int id )
    {
        auto it = std::ranges::find_if( m_nodes, [ id ]( const CShaderNode* pNode ) {
            return pNode->GetId() == id;
        } );
        if ( it == m_nodes.cend() )
        {
            return nullptr;
        }
        return *it;
    }

    void Serialize( const char* filename ) const;
    void Deserialize( const char* filename ) const;

    void DrawNodes() const noexcept;
    bool Compile( const SFile& file, int cullMode );
    std::vector<SShaderBinding>& GetBindings();
    void PushData() const noexcept;

private:
    std::list<CShaderNode*> m_nodes;
    SShaderInfo m_shaderInfo {};

    int m_nodeId {};

    void PullNodeData() noexcept;
    static shaderc_shader_kind Convert( shader_stage stage ) noexcept;
    std::string Compile( const CShaderNode* pNode, std::vector<const CShaderNode*>& evaluatedNodes, std::unordered_map<std::string_view, int>& outputVarCount, SShaderInfo& shaderInfo );
    std::string ConvertToGlslUniform( const SShaderBinding& binding );
};
