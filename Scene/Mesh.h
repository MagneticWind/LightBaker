#ifndef MESH_H
#define MESH_H

#include "Math\Vector3f.h"
#include "Math\Vector2f.h"
#include "Math\Vector4f.h"
#include "Math\AABB.h"
#include "IGeometry.h"

namespace Magnet
{
namespace Scene
{
enum VertexDecl
{
	POSITION,
	NORMAL,
	TEXCOORD,
	COLOR,
	TANGENT,
	BINORMAL,
	MAX_DESC_COUNT
};

class Mesh : public IGeometry
{
public:
	Mesh(const char* pMeshName);
	virtual ~Mesh();

	void SetNumVerts(int iNumVerts);
	void SetNumFaces(int iNumFaces);

	int GetNumVerts() const;
	int GetNumFaces() const;
	int GetStride() const;
	void GetDecls(int& iDeclCount, VertexDecl* pDecls) const;
	const void* GetVertexBufferPtr() const;
	const void* GetIndexBuffePtr() const;

	const char* GetMeshName() const;
	const Math::AABB& GetBBox() const;

	virtual PrimitiveType GetPrimitiveType() const;
	virtual GeometryType GetGeometryType() const;
	virtual const char* GetName() const;

	virtual int GetNumPrimitives() const;
	virtual bool IsLoaded() const;
	virtual void SetLoaded(bool bLoaded);

	void AddVertexDecl(VertexDecl eDecl);

	void GetVetexDecls(int& iSize, VertexDecl* pDecls) const;

	float* CreateVertexDataBuffer(int iNumVertices, int iNumFloats);
	unsigned int* CreateIndexDataBuffer(int iNumFaces);

protected:
	char m_pName[256];
	void* m_pVertexDataBuffer;
	void* m_pIndexDataBuffer;
	int m_iNumVertices;
	int m_iNumFaces;

	bool m_bIsLoaded;

	int m_iStride;

	VertexDecl m_eaVertexDecls[MAX_DESC_COUNT];
	int m_iNumDecls;

	Math::AABB m_BBox;
};

inline void Mesh::AddVertexDecl(VertexDecl eDecl)
{
	m_eaVertexDecls[m_iNumDecls++] = eDecl;
}

inline void Mesh::GetVetexDecls(int& iSize, VertexDecl* pDecls) const
{
	iSize = m_iNumDecls;
	for (int i = 0; i < iSize; ++i)
		pDecls[i] = m_eaVertexDecls[i];
}

inline const void* Mesh::GetVertexBufferPtr() const
{
	return m_pVertexDataBuffer;
}

inline const void* Mesh::GetIndexBuffePtr() const
{
	return m_pIndexDataBuffer;
}

} // namespace Scene
} //namespace Magnet

#endif