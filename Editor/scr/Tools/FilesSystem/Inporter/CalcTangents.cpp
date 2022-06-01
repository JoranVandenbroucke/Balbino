
#include "CalcTangents.h"

CCalcTangents::CCalcTangents()
{
	m_iface.m_getNumFaces = GetNumFaces;
	m_iface.m_getNumVerticesOfFace = GetNumVerticesOfFace;

	m_iface.m_getNormal = GetNormal;
	m_iface.m_getPosition = GetPosition;
	m_iface.m_getTexCoord = GetTexCoords;
	m_iface.m_setTSpaceBasic = SetTspaceBasic;

	m_context.m_pInterface = &m_iface;
}

void CCalcTangents::Calc( STempMesh* mesh )
{
	m_context.m_pUserData = mesh;

	genTangSpaceDefault( &this->m_context );
}

int CCalcTangents::GetVertexIndex( const SMikkTSpaceContext* context, int iFace, int iVert )
{
	const STempMesh* workingMesh = static_cast<STempMesh*>( context->m_pUserData );
	const auto faceSize = GetNumVerticesOfFace( context, iFace );
	const auto indicesIndex = ( iFace * faceSize ) + iVert;
	const int index = workingMesh->indices[indicesIndex];

	return index;
}

int CCalcTangents::GetNumFaces( const SMikkTSpaceContext* context )
{
	const STempMesh* workingMesh = static_cast<STempMesh*>( context->m_pUserData );
	const float fSize = static_cast<float>( workingMesh->indices.size() ) / 3.f;
	const int iSize = static_cast<int>( workingMesh->indices.size() ) / 3;

	assert( ( fSize - ( float ) iSize ) == 0.f );
	( void ) fSize;
	return iSize;
}

int CCalcTangents::GetNumVerticesOfFace( const SMikkTSpaceContext* context, int iFace )
{
	( void ) context;
	( void ) iFace;
	return 3;
}

void CCalcTangents::GetPosition( const SMikkTSpaceContext* context, float outpos[], int iFace, int iVert )
{
	const STempMesh* workingMesh = static_cast<STempMesh*>( context->m_pUserData );

	const auto index = GetVertexIndex( context, iFace, iVert );
	const auto vertex = workingMesh->vertices[index];

	outpos[0] = vertex.position.x;
	outpos[1] = vertex.position.y;
	outpos[2] = vertex.position.z;
}

void CCalcTangents::GetNormal( const SMikkTSpaceContext* context, float outnormal[], int iFace, int iVert )
{
	const STempMesh* workingMesh = static_cast<STempMesh*>( context->m_pUserData );

	const auto index = GetVertexIndex( context, iFace, iVert );
	const auto vertex = workingMesh->vertices[index];

	outnormal[0] = vertex.normal.x;
	outnormal[1] = vertex.normal.y;
	outnormal[2] = vertex.normal.z;
}

void CCalcTangents::GetTexCoords( const SMikkTSpaceContext* context, float outuv[], int iFace, int iVert )
{
	auto working_mesh = static_cast<STempMesh*>( context->m_pUserData );

	auto index = GetVertexIndex( context, iFace, iVert );
	auto vertex = working_mesh->vertices[index];


	outuv[0] = vertex.uv.x;
	outuv[1] = vertex.uv.y;
}

void CCalcTangents::SetTspaceBasic( const SMikkTSpaceContext* context, const float tangentu[], float fSign, int iFace, int iVert )
{
	auto workingMesh = static_cast<STempMesh*>( context->m_pUserData );


	const auto index = GetVertexIndex( context, iFace, iVert );
	auto* vertex = &workingMesh->vertices[index];

	vertex->tangent.x = tangentu[0];
	vertex->tangent.y = tangentu[1];
	vertex->tangent.z = tangentu[2];
	vertex->tangent.w = fSign;
}
