#include <assert.h>

#include "External\IL\il.h"
#include "IGeometry.h"
#include "Math\Vector4f.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Texture.h"


namespace Magnet
{

namespace Scene
{

namespace ResourceManagerPrivate
{
	char MESH_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\data\\mesh\\";
	char TEXTURE_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\data\\texture\\";
}

ResourceManager* ResourceManager::ms_pInstance = 0;

//------------------------------------------------------------------
ResourceManager::ResourceManager()
{
	ilInit();
}

//------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	std::list<IGeometry*>::iterator itGeo = m_lGeometrys.begin();
	std::list<IGeometry*>::iterator itGeoEnd = m_lGeometrys.end();
	while (itGeo != itGeoEnd)
	{
		IGeometry* pGeometry = *itGeo;
		delete pGeometry;
		pGeometry = 0;
		itGeo++;
	}
	m_lGeometrys.clear();

	std::list<Texture*>::iterator itTex = m_lTextures.begin();
	std::list<Texture*>::iterator itTexEnd = m_lTextures.end();

	while (itTex != itTexEnd)
	{
		Texture* pTexture = *itTex;
		delete pTexture;
		pTexture = 0;
		itTex++;
	}
	m_lTextures.clear();
}

//------------------------------------------------------------------
ResourceManager& ResourceManager::GetInstance()
{
	assert(ms_pInstance != 0);
	return *ms_pInstance;
}

//------------------------------------------------------------------
bool ResourceManager::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void ResourceManager::Initialize()
{
	assert(ms_pInstance == 0);
	ms_pInstance = new ResourceManager();
}

//------------------------------------------------------------------
void ResourceManager::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;
}

//------------------------------------------------------------------
void ResourceManager::Process()
{
	LoadResources();
}

//------------------------------------------------------------------
void ResourceManager::LoadResources()
{

	// initialize callback function
	//Render::RenderManager::GetInstance().SetLoadShaderCallBack(&LoadShader);
	//Render::RenderManager::GetInstance().SetLoadMeshCallBack(&LoadMesh);

	// mesh
	std::list<IGeometry*>::iterator itGeo = m_lGeometrys.begin();
	std::list<IGeometry*>::iterator itGeoEnd = m_lGeometrys.end();

	while (itGeo != itGeoEnd)
	{
		IGeometry* pGeometry = *itGeo;

		if (!pGeometry->IsLoaded())
		{
			if (pGeometry->GetGeometryType() == GEOMETRY_MESH)
			{
				LoadMesh(static_cast<Mesh*>(pGeometry));
			}
		}

		itGeo++;
	}

	//texture
	std::list<Texture*>::iterator itTexture = m_lTextures.begin();
	std::list<Texture*>::iterator itTextureEnd = m_lTextures.end();
	while (itTexture != itTextureEnd)
	{
		Texture* pTexture = *itTexture;

		if (!pTexture->IsLoaded())
		{
			LoadTexture(pTexture);
			pTexture->SetLoaded(true);
		}
		itTexture++;
	}

}

//------------------------------------------------------------------
void ResourceManager::SetMeshFolderPath(const char* pFolderPath)
{
	int iStrLen = strlen(pFolderPath);
	for (int i = 0; i < iStrLen; ++i)
		m_caMeshFolderPath[i] = pFolderPath[i];
	m_caMeshFolderPath[iStrLen] = '\0';
}

//------------------------------------------------------------------
void ResourceManager::SetTextureFolderPath(const char* pFolderPath)
{
	int iStrLen = strlen(pFolderPath);
	for (int i = 0; i < iStrLen; ++i)
		m_caTextureFolderPath[i] = pFolderPath[i];
	m_caTextureFolderPath[iStrLen] = '\0';
}

//------------------------------------------------------------------
void ResourceManager::AddTexture(Texture* pTexture)
{
	m_lTextures.push_back(pTexture);
}

//------------------------------------------------------------------
Texture* ResourceManager::FindTexture(const char* pTextureName)
{
	std::list<Texture*>::iterator it = m_lTextures.begin();
	std::list<Texture*>::iterator itEnd = m_lTextures.end();

	while (it != itEnd)
	{
		Texture* pTexture = *it;
		if (strcmp(pTexture->GetName(), pTextureName) == 0)
		{
			return pTexture;
		}
		it++;
	}

	return 0;
}

//-------------------------------------------------------------------
IGeometry* ResourceManager::FindGeometry(const char* pGeometryName)
{
	std::list<IGeometry*>::iterator it = m_lGeometrys.begin();
	std::list<IGeometry*>::iterator itEnd = m_lGeometrys.end();

	while (it != itEnd)
	{
		IGeometry* pGeometry = *it;
		if (strcmp(pGeometry->GetName(), pGeometryName) == 0)
		{
			return pGeometry;
		}
		it++;
	}

	return 0;
}

//------------------------------------------------------------------
void ResourceManager::AddGeometry(IGeometry* pGeometry)
{
	m_lGeometrys.push_back(pGeometry);
}

//------------------------------------------------------------------
void ResourceManager::LoadTexture(Texture* pTexture)
{
	char caPath[256];
	strcpy(caPath, ResourceManagerPrivate::TEXTURE_PATH);
	strcat(caPath, pTexture->GetName());

	// Platform dependent. For example, dds files, used as cubemaps, DX formats
	//if (strstr(pTexture->GetName(), ".dds") != 0)
	//{
	//	//Render::RenderManager::GetInstance().LoadTextureResource(caPath, pTexture);

	//	return;
	//}

	// load images using IL

	unsigned int uTextureID;
	ilGenImages(1, &uTextureID);
	ilBindImage(uTextureID);

	// the formatting setting influences of Texture loading, so must be set up before loading
	ilEnable(IL_FORMAT_SET);
	ilSetInteger(IL_FORMAT_MODE, IL_RGBA);

	if (!ilLoadImage(caPath))
	{
		assert(0);
	}

	int iWidth = ilGetInteger(IL_IMAGE_WIDTH);
	int iHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	pTexture->SetDimension(iWidth, iHeight);
	pTexture->SetFormat(R8G8B8A8_UINT);

	void* pData = pTexture->CreateDataBuffer();

	unsigned char* pDataUINT8 = static_cast<unsigned char*>(pData);
	unsigned char* pImageData = ilGetData();
	for (int y = 0; y < iHeight; y++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			int i = (y * iWidth + x) * 4;
			pDataUINT8[i] = pImageData[i];
			pDataUINT8[i + 1] = pImageData[i + 1];
			pDataUINT8[i + 2] = pImageData[i + 2];
			pDataUINT8[i + 3] = pImageData[i + 3];
		}
	}

	ilDeleteImage(uTextureID);
}

//------------------------------------------------------------------
void ResourceManager::LoadMeshPly(Mesh* pMesh)
{
	char filePath[256];
	strcpy(filePath, ResourceManagerPrivate::MESH_PATH);
	strcat(filePath, pMesh->GetName());

	FILE *pFile = fopen(filePath, "r");
	if (pFile == 0)
	{
		printf("can't find the mesh %s\n", filePath);
		assert(0);
	}

	char line[256];

	fgets(line, 256, pFile);
	if (strncmp(line, "ply", strlen("ply")))
	{
		printf("no magic word");
		assert(0);
	}

	bool bRecognizedAttributes[32]; // offsets
	int iAttributesCount = 0;

	// vertex buffer
	int iNumFloats = 0;
	int iNumVertices = 0;
	int iNumFaces = 0;

	while (1)
	{
		fgets(line, 256, pFile);

		if (!strncmp(line, "comment", strlen("comment")))
		{
			continue;
		}
		else if (!strncmp(line, "end_header", strlen("end_header")))
		{
			break;
		}
		else if (!strncmp(line, "format", strlen("format")))
		{
			//@TODO: support other formats
			continue;
		}
		else if (!strncmp(line, "element", strlen("element")))
		{
			if (sscanf(line, "element vertex %d", &iNumVertices) == 1)
			{
				/* correct */
			}
			else if (sscanf(line, "element face %d", &iNumFaces) == 1)
			{
				/* correct */
			}
			else
			{
				printf("PlyMesh::loadMesh(): element unrecognized.");
				assert(0);
			}
		}
		else if (!strncmp(line, "property", strlen("property")))
		{

			if (!strcmp(line, "property float x\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float y\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float z\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;

				pMesh->AddVertexDecl(POSITION);

				continue;
			}

			if (!strcmp(line, "property float nx\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float ny\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float nz\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;

				pMesh->AddVertexDecl(NORMAL);

				//m_bHasNormal = true;

				continue;
			}

			if (!strcmp(line, "property float tx\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float ty\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float tz\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;

				pMesh->AddVertexDecl(TANGENT);

				continue;
			}

			if (!strcmp(line, "property float bx\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float by\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float bz\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;

				pMesh->AddVertexDecl(BINORMAL);

				continue;
			}

			if (!strcmp(line, "property float s\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;
				continue;
			}

			if (!strcmp(line, "property float t\n"))
			{
				bRecognizedAttributes[iAttributesCount++] = true;
				iNumFloats++;

				pMesh->AddVertexDecl(TEXCOORD);

				continue;
			}

			if (!strcmp(line, "property list uchar int vertex_indices\n"))
			{
				/* face list recognized */
				continue;
			}

			if (!strcmp(line, "property list uchar uint vertex_indices\n"))
			{
				/* face list recognized */
				continue;
			}

			// This property is not expected, will be ignored when parsing vertices
			bRecognizedAttributes[iAttributesCount++] = false;
		}
	}

	pMesh->SetNumVerts(iNumVertices);
	pMesh->SetNumFaces(iNumFaces);

	float* pVertexData = pMesh->CreateVertexDataBuffer(iNumVertices, iNumFloats);
	unsigned int* pIndexData = pMesh->CreateIndexDataBuffer(iNumFaces);

	// parse vertices
	for (int i = 0; i < iNumVertices; ++i)
	{
		float temp;
		int iCount = 0;
		for (int j = 0; j < iAttributesCount; j++)
		{
			if (bRecognizedAttributes[j])
			{
				fscanf(pFile, "%f", pVertexData + i * iNumFloats + iCount);
				iCount++;
			}
			else
				fscanf(pFile, "%f", &temp);
		}
	}

	// parse indices
	int iCount = 0;
	unsigned int uFaceVertices = 0;
	for (int i = 0; i < iNumFaces; ++i)
	{
		fscanf(pFile, "%u %u %u %u", &uFaceVertices, pIndexData + iCount, pIndexData + iCount + 1, pIndexData + iCount + 2);
		assert(uFaceVertices == 3);
		iCount += 3;
	}

	pMesh->SetLoaded(true);
}

//------------------------------------------------------------------
void ResourceManager::LoadMeshObj(Mesh* pMesh)
{

}

//------------------------------------------------------------------
void ResourceManager::LoadMesh(Mesh* pMesh)
{
	const char* pMeshName = pMesh->GetName();
	const int iStrLen = strlen(pMeshName);
	if (pMeshName[iStrLen - 3] == 'p' && pMeshName[iStrLen - 2] == 'l' && pMeshName[iStrLen - 1] == 'y')
	{
		LoadMeshPly(pMesh);
	}
	else if (pMeshName[iStrLen - 3] == 'o' && pMeshName[iStrLen - 2] == 'b' && pMeshName[iStrLen - 1] == 'j')
	{
		LoadMeshObj(pMesh);
	}



}

} // namespace Scene
} // namespace Magnet