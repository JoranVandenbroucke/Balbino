//
// Created by joran on 25/02/2023.
//

#ifndef BALBINO_SHADERGRAPH_H
#define BALBINO_SHADERGRAPH_H
#include <list>
#include <set>
#include "Nodes/ShaderNode.h"
#include "shaderc/shaderc.h"
#include "FileParcer.h"

class CShaderGraph
{
public:
    CShaderGraph() = default;
    ~CShaderGraph() = default;
    CShaderGraph( const CShaderGraph& ) = delete;
    CShaderGraph( CShaderGraph&& ) = delete;
    CShaderGraph& operator=( const CShaderGraph& ) = delete;
    CShaderGraph& operator=( CShaderGraph&& ) = delete;
    
    void Initialize();
    void Cleanup();
    
    CShaderNode *Add( CShaderNode *node);
    
    template<typename T, typename... Args> T *CreateNode(Args &&...args)
    {
        return new T(m_nodeId, args...);
    }
    template<typename T> void DeleteNode(T *node)
    {
        delete node;
    }
    
    void Connect(int fromNode, int fromAttr,int toNode, int toAttr);
    void Connect(CShaderOutput *from, CShaderInput *to);
    void Disconnect(int fromNode, int fromAttr,int toNode, int toAttr);
    void Disconnect(CShaderOutput *from);
    void Disconnect(CShaderInput *to);
    
    void Serialize(const char* filename);
    void Deserialize(const char* filename);
    
    void DrawNodes();
    bool Compile( const SFile& file, int cullMode );
    std::vector<SShaderBinding>& GetBindings();
    void PushData();
private:
    std::list<CShaderNode*> m_nodes;
    SShaderInfo             m_shaderInfo;
    
    int m_nodeId{};
    
    void PullNodeData();
    shaderc_shader_kind Convert( shader_stage stage);
    std::string Compile( const CShaderNode* pNode, std::vector<const CShaderNode*>& evaluatedNodes, std::unordered_map<std::string, int>& outputVarCount, SShaderInfo& shaderInfo );
    std::string ConvertToGlslUniform( const SShaderBinding& binding);
};

#endif //BALBINO_SHADERGRAPH_H