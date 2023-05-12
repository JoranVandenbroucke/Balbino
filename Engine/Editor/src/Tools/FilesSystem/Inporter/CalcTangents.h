#pragma once
#include <vector>

#include "mikktspace.h"
#include "Vertex.h"

struct STempMesh
{
    std::vector<FawnVision::SVertex> vertices{};
    std::vector<uint32_t>         indices{};
};

class CCalcTangents
{
public:
    CCalcTangents();
    void Calc( STempMesh* mesh );

private:
    SMikkTSpaceInterface m_iface{};
    SMikkTSpaceContext   m_context{};
    
    static int GetVertexIndex( const SMikkTSpaceContext* context, int iFace, int iVert );
    static int GetNumFaces( const SMikkTSpaceContext* context );
    static int GetNumVerticesOfFace( const SMikkTSpaceContext* context, int iFace );
    static void GetPosition( const SMikkTSpaceContext* context, float outpos[], int iFace, int iVert );
    static void GetNormal( const SMikkTSpaceContext* context, float outnormal[], int iFace, int iVert );
    static void GetTexCoords( const SMikkTSpaceContext* context, float outuv[], int iFace, int iVert );
    static void SetTspaceBasic( const SMikkTSpaceContext* context, const float tangentu[], float fSign, int iFace, int iVert );
};
