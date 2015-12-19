#include <assert.h>

#include "Camera.h"
#include "Entity.h"
#include "External\tinyxml2\tinyxml2.h"
#include "Light.h"
#include "Material.h"
#include "MaterialSky.h"
#include "Memory.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "SceneNode.h"
#include "Surface.h"

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
SceneLoader::SceneLoader() : m_pCurrentLoadingScene(0), m_bFinishedLoading(false)
{

}

//------------------------------------------------------------------
void SceneLoader::StartLoadingScene(Scene* const pScene)
{
	//Core::DebugInfo::GetInstance().PrintInfo("<LOG>: SceneLoader::StartLoadingScene...");
	m_pCurrentLoadingScene = pScene;
	LoadSceneGraph(pScene->GetName());
}

//------------------------------------------------------------------
bool SceneLoader::LoadSceneGraph(const char* pSceneFile)
{
	FILE *pFile = fopen(pSceneFile, "r");
	fseek(pFile, 0, SEEK_END);
	int iSize = ftell(pFile);
	rewind(pFile);
	char* pData = new char[iSize];
	int iResult = fread(pData, 1, iSize, pFile);

	tinyxml2::XMLDocument doc;
	doc.Parse(pData);

	// create a root scene node
	SceneNode* pRootNode = new SceneNode();
	m_pCurrentLoadingScene->SetSceneRoot(pRootNode);

	ParseSceneNode(doc.FirstChildElement("scenenode"), pRootNode);

	fclose(pFile);

	m_bFinishedLoading = true;
	return true;
}

//------------------------------------------------------------------
void SceneLoader::ParseSceneNode(tinyxml2::XMLElement* pElement, SceneNode* pParentNode)
{
	tinyxml2::XMLElement* pChildElement = pElement->FirstChildElement("scenenode");

	if (pChildElement == 0)
	{
		pParentNode->SetLeaf(true);
		ParseEntity(pElement, pParentNode);
		return;
	}

	while (pChildElement)
	{
		SceneNode* childNode = new SceneNode();
		pParentNode->AddChildNode(childNode);
		ParseSceneNode(pChildElement, childNode);

		pChildElement = pChildElement->NextSiblingElement("scenenode");
	}
}

//------------------------------------------------------------------
void SceneLoader::ParseEntity(tinyxml2::XMLElement* pElement, SceneNode* pNode)
{
	tinyxml2::XMLElement* pEntityElement = pElement->FirstChildElement("entity");
	const char* name = pEntityElement->FirstChildElement("name")->GetText();
	const char* type = pEntityElement->FirstChildElement("type")->GetText();

	if (strcmp(type, "camera") == 0)
	{
		Camera* pCamera = new Camera(name);
		ParseCamera(pEntityElement, pCamera);
		pNode->SetEntity(pCamera);
	}
	else if (strcmp(type, "light") == 0)
	{
		Light* pLight = new Light(name);
		ParseLight(pEntityElement, pLight);
		pNode->SetEntity(pLight);
	}
	else if (strcmp(type, "normal") == 0)
	{
		Entity* pModel = new Entity(name);
		ParseEntity(pEntityElement, pModel);
		pNode->SetEntity(pModel);
	}

}

//------------------------------------------------------------------
void SceneLoader::ParseCamera(tinyxml2::XMLElement* pElement, Camera* pCamera)
{
	const char* position = pElement->FirstChildElement("position")->GetText();
	const char* lookat = pElement->FirstChildElement("lookat")->GetText();
	float fFov, fVelocity, fRotationFactor;
	pElement->FirstChildElement("fov")->QueryFloatText(&fFov);
	pElement->FirstChildElement("velocity")->QueryFloatText(&fVelocity);
	pElement->FirstChildElement("rotationfactor")->QueryFloatText(&fRotationFactor);
	Math::Vector3f v3Position, v3Lookat;
	sscanf(position, "%f %f %f", &v3Position.x, &v3Position.y, &v3Position.z);
	sscanf(lookat, "%f %f %f", &v3Lookat.x, &v3Lookat.y, &v3Lookat.z);

	pCamera->SetFOV(fFov);
	pCamera->SetPosition(v3Position);
	pCamera->SetLookat(v3Lookat);
	pCamera->SetVelocity(fVelocity);

}

//------------------------------------------------------------------
void SceneLoader::ParseLight(tinyxml2::XMLElement* pElement, Light* pLight)
{
	const char* type = pElement->FirstChildElement("lighttype")->GetText();
	if (strcmp(type, "directional") == 0)
	{
		Math::Vector3f v3Direction, v3Color;
		const char* direction = pElement->FirstChildElement("direction")->GetText();
		const char* color = pElement->FirstChildElement("color")->GetText();
		sscanf(direction, "%f %f %f", &v3Direction.x, &v3Direction.y, &v3Direction.z);
		sscanf(color, "%f %f %f", &v3Color.x, &v3Color.y, &v3Color.z);
		pLight->SetType(LIGHT_DIRECTIONAL);
		pLight->SetDirection(v3Direction);
		pLight->SetColor(v3Color);
	}
	else if (strcmp(type, "point") == 0)
	{
		Math::Vector3f v3Position, v3Color;
		const char* direction = pElement->FirstChildElement("position")->GetText();
		const char* color = pElement->FirstChildElement("color")->GetText();
		sscanf(direction, "%f %f %f", &v3Position.x, &v3Position.y, &v3Position.z);
		sscanf(color, "%f %f %f", &v3Color.x, &v3Color.y, &v3Color.z);
		pLight->SetType(LIGHT_POINT);
		pLight->SetPosition(v3Position);
		pLight->SetColor(v3Color);
	}
}

//------------------------------------------------------------------
void SceneLoader::ParseEntity(tinyxml2::XMLElement* pElement, Entity* pModel)
{
	const char* name = pElement->FirstChildElement("name")->GetText();
	const char* type = pElement->FirstChildElement("type")->GetText();
	if (strcmp(type, "normal") == 0)
	{
		IRenderObject* pObject = ParseRenderObject(pElement->FirstChildElement("renderobject"));
		pModel->SetRenderObject(pObject);
	}
}


//------------------------------------------------------------------
IRenderObject* SceneLoader::ParseRenderObject(tinyxml2::XMLElement* pElement)
{
	const char* type = pElement->FirstChildElement("type")->GetText();
	IRenderObject* pObject = 0;
	if (type)
	{
		if (strcmp(type, "normal") == 0)
		{
			pObject = new RenderObject();
		}
	}
	else
	{
		printf("Element type expected.\n");
		assert(0);
	}

	tinyxml2::XMLElement* pTransform = pElement->FirstChildElement("transform");

	if (pTransform)
	{
		ParseTransformation(pTransform, pObject);
	}

	tinyxml2::XMLElement* pSurfaceElement = pElement->FirstChildElement("surface");
	while (pSurfaceElement != 0)
	{
		Surface* pSurface = new Surface();
		ParseSurface(pSurfaceElement, pSurface);
		pObject->AddSurface(pSurface);

		pSurfaceElement = pSurfaceElement->NextSiblingElement("surface");
	}

	return pObject;
}

//------------------------------------------------------------------
void SceneLoader::ParseTransformation(tinyxml2::XMLElement* pElement, IRenderObject* pObject)
{
	tinyxml2::XMLElement* pTransformElement = pElement->FirstChildElement();
	while (pTransformElement)
	{
		const char* pProperty = pTransformElement->Name();
		if (strcmp(pProperty, "translate") == 0)
		{
			const char* pTranslate = pTransformElement->GetText();
			Math::Vector3f v3Translate;
			sscanf(pTranslate, "%f %f %f", &v3Translate.x, &v3Translate.y, &v3Translate.z);
			pObject->SetTranslation(v3Translate);
		}
		else if (strcmp(pProperty, "scale") == 0)
		{
			const char* pScale = pTransformElement->GetText();
			Math::Vector3f v3Scale;
			sscanf(pScale, "%f %f %f", &v3Scale.x, &v3Scale.y, &v3Scale.z);
			pObject->SetScale(v3Scale);
		}
		else if (strcmp(pProperty, "rotate") == 0)
		{
			const char* pRotate = pTransformElement->GetText();
			Math::Vector3f v3Rotate;
			sscanf(pRotate, "%f %f %f", &v3Rotate.x, &v3Rotate.y, &v3Rotate.z);
			pObject->SetRotation(v3Rotate);
		}
		pTransformElement = pTransformElement->NextSiblingElement();
	}
}

//------------------------------------------------------------------
void SceneLoader::ParseSurface(tinyxml2::XMLElement* pElement, Surface* pSurface)
{
	tinyxml2::XMLElement* pMesh = pElement->FirstChildElement("mesh");

	if (pMesh)
	{
		const char* pMeshName = pMesh->GetText();
		IGeometry* pGeometry = ResourceManager::GetInstance().FindGeometry(pMeshName);
		if (pGeometry == 0)
		{
			Mesh* pNewMesh = new Mesh(pMeshName);
			pSurface->SetGeometry(pNewMesh);
			ResourceManager::GetInstance().AddGeometry(pNewMesh);
		}
	}

	tinyxml2::XMLElement* pMaterialElement = pElement->FirstChildElement("material");
	if (pMaterialElement)
	{
		const char* type = pMaterialElement->FirstChildElement("type")->GetText();
		const char* tech = pMaterialElement->FirstChildElement("tech")->GetText();
		int techValue;
		sscanf(tech, "%d", &techValue);

		if (strcmp(type, "normal") == 0)
		{
			Material* pMaterial = new Material(static_cast<unsigned char>(techValue));
			ParseMaterial(pMaterialElement, pMaterial);
			pSurface->SetMaterial(pMaterial);
		}
		else if (strcmp(type, "sky") == 0)
		{
			MaterialSky* pMaterial = new MaterialSky(static_cast<unsigned char>(techValue));
			ParseMaterial(pMaterialElement, pMaterial);
			pSurface->SetMaterial(pMaterial);
		}
		else
		{
			printf("material is not supported.");
			assert(0);
		}
	}

	tinyxml2::XMLElement* pTextureElement = pElement->FirstChildElement("texture");

	while (pTextureElement != 0)
	{
		const char* pTextureName = pTextureElement->GetText();
		Texture* pTexture = ResourceManager::GetInstance().FindTexture(pTextureName);
		if (pTexture == 0)
		{
			Texture* pNewTexture = new Texture(pTextureName);
			ResourceManager::GetInstance().AddTexture(pNewTexture);
			pTexture = pNewTexture;
		}
		pSurface->AddTexture(pTexture);

		pTextureElement = pTextureElement->NextSiblingElement("texture");

	}

	tinyxml2::XMLElement* pShaderElement = pElement->FirstChildElement("shader");
}

void SceneLoader::ParseMaterial(tinyxml2::XMLElement* pElement, IMaterial* pIMaterial)
{
	if (pIMaterial->GetType() == MATERIAL_NORMAL)
	{
		Material* pMaterial = static_cast<Material*>(pIMaterial);
		tinyxml2::XMLElement* pMaterialElement = pElement->FirstChildElement();
		while (pMaterialElement)
		{
			const char* pProperty = pMaterialElement->Name();
			if (strcmp(pProperty, "ambient") == 0)
			{
				const char* pAmbient = pMaterialElement->GetText();
				Math::Vector4f v4Ambient;
				sscanf(pAmbient, "%f %f %f %f", &v4Ambient.x, &v4Ambient.y, &v4Ambient.z, &v4Ambient.w);
				pMaterial->SetAmbient(v4Ambient);

			}
			else if (strcmp(pProperty, "diffuse") == 0)
			{
				const char* pDiffuse = pMaterialElement->GetText();
				Math::Vector4f v4Diffuse;
				sscanf(pDiffuse, "%f %f %f %f", &v4Diffuse.x, &v4Diffuse.y, &v4Diffuse.z, &v4Diffuse.w);
				pMaterial->SetDiffuse(v4Diffuse);
			}
			else if (strcmp(pProperty, "specular") == 0)
			{
				const char* pSpecular = pMaterialElement->GetText();
				Math::Vector4f v4Specular;
				sscanf(pSpecular, "%f %f %f %f", &v4Specular.x, &v4Specular.y, &v4Specular.z, &v4Specular.w);
				pMaterial->SetSpecular(v4Specular);
			}
			else if (strcmp(pProperty, "exponent") == 0)
			{
				float fExponent;
				pMaterialElement->QueryFloatText(&fExponent);
				pMaterial->SetExponent(fExponent);
			}
			pMaterialElement = pMaterialElement->NextSiblingElement();
		}
	}
	else if (pIMaterial->GetType() == MATERIAL_SKY)
	{
		MaterialSky* pMaterial = static_cast<MaterialSky*>(pIMaterial);
		tinyxml2::XMLElement* pMaterialElement = pElement->FirstChildElement();
		while (pMaterialElement)
		{
			const char* pProperty = pMaterialElement->Name();
			if (strcmp(pProperty, "type") == 0)
			{
				const char* pType = pMaterialElement->GetText();

				if (strcmp(pType, "cubemap") == 0)
				{
					pMaterial->SetSkyType(SKY_CUBE);
				}
				else if (strcmp(pType, "parabloid") == 0)
				{
					pMaterial->SetSkyType(SKY_PARABLOID);
				}
			}
			else if (strcmp(pProperty, "map") == 0)
			{
				const char* pName = pMaterialElement->GetText();
				pMaterial->SetTextureName(pName);
			}
			pMaterialElement = pMaterialElement->NextSiblingElement();
		}
	}
	
}

} // namespace Scene
} // namespace Magnet