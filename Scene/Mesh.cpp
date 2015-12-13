#include "Mesh.h"
#include <stdlib.h>
#include <string.h>

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
Mesh::Mesh(const char* pName)
{
	strcpy(m_pName, pName);
	m_pVertexDataBuffer = 0;
	m_pIndexDataBuffer = 0;
	m_iNumVertices = 0;
	m_iNumFaces = 0;
	m_iNumDecls = 0;

	m_iStride = 0;

	m_bIsLoaded = false;
}

//------------------------------------------------------------------
Mesh::~Mesh()
{

}

//------------------------------------------------------------------
void Mesh::SetNumVerts(int iNumVerts)
{
	m_iNumVertices = iNumVerts;
}

//------------------------------------------------------------------
void Mesh::SetNumFaces(int iNumFaces)
{
	m_iNumFaces = iNumFaces;
}

//------------------------------------------------------------------
int Mesh::GetNumVerts() const
{
	return m_iNumVertices;
}

//------------------------------------------------------------------
int Mesh::GetNumFaces() const
{
	return m_iNumFaces;
}

//------------------------------------------------------------------
int Mesh::GetStride() const
{
	return m_iStride;
}

//------------------------------------------------------------------
void Mesh::GetDecls(int& iDeclCount, VertexDecl* pDecls) const
{
	iDeclCount = m_iNumDecls;
	for (int i = 0; i < iDeclCount; ++i)
	{
		pDecls[i] = m_eaVertexDecls[i];
	}
}

//------------------------------------------------------------------
const char* Mesh::GetMeshName() const
{
	return m_pName;
}

//------------------------------------------------------------------
const Math::AABB& Mesh::GetBBox() const
{
	return m_BBox;
}

//------------------------------------------------------------------
PrimitiveType Mesh::GetPrimitiveType() const
{
	return PRIMITIVE_TRIANGLE_LIST;
}

//------------------------------------------------------------------
GeometryType Mesh::GetGeometryType() const
{
	return GEOMETRY_MESH;
}

//------------------------------------------------------------------
const char* Mesh::GetName() const
{
	return m_pName;
}

//------------------------------------------------------------------
bool Mesh::IsLoaded() const
{
	return m_bIsLoaded;
}

//------------------------------------------------------------------
void Mesh::SetLoaded(bool bLoaded)
{
	m_bIsLoaded = bLoaded;
}

//------------------------------------------------------------------
int Mesh::GetNumPrimitives() const
{
	return m_iNumFaces;
}

//------------------------------------------------------------------
float* Mesh::CreateVertexDataBuffer(int iNumVertices, int iVertexByteSize)
{
	m_pVertexDataBuffer = malloc(iNumVertices * iVertexByteSize);
	return static_cast<float*>(m_pVertexDataBuffer);
}

//------------------------------------------------------------------
unsigned int* Mesh::CreateIndexDataBuffer(int iNumFaces)
{
	m_pIndexDataBuffer = malloc(iNumFaces * 3 * sizeof(unsigned int));
	return static_cast<unsigned int*>(m_pIndexDataBuffer);
}

} // namespace Scene
} // namespace Magnet