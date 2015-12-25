#include <stdio.h>
#include <string.h>

#include "Camera.h"
#include "IEntity.h"
#include "IRenderObject.h"
#include "Light.h"
#include "Math\Intersection.h"
#include "Math\Matrix4f.h"
#include "Memory.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Surface.h"

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
Scene::Scene(const char* pSceneName)
{
	int iStrLen = strlen(pSceneName);
	for (int i = 0; i < iStrLen; ++i)
		m_caSceneName[i] = pSceneName[i];
	m_caSceneName[iStrLen] = '\0';

	m_pCurrentCamera = 0;
	m_pSceneRoot = 0;

	// initialize screen space camera
	m_pQuadCamera = new Camera("quad");
}

//------------------------------------------------------------------
void Scene::Update(float fTimeLapse)
{
	// update scene graph
	m_lCameras.clear();
	m_lLights.clear();
	m_lEntities.clear();
	m_lRenderObjects.clear();
	Math::Matrix4f mIdentity; // default constructor creates identity matrix
	TraverseSceneGraph(m_pSceneRoot, mIdentity);

	m_pCurrentCamera = *m_lCameras.begin();

	// update entities
	std::list<IEntity*>::iterator it = m_lEntities.begin();
	while (it != m_lEntities.end())
	{
		IEntity* pEntity = *it;
		if (pEntity->IsActive())
			pEntity->Update(fTimeLapse);
		it++;
	}
}

//------------------------------------------------------------------
// mat is the concatenated transformation for current entity/scenenode
void Scene::TraverseSceneGraph(SceneNode* pNode, Math::Matrix4f& mat)
{

	if (pNode->m_bLeaf == true)
	{
		IEntity* const pEntity = pNode->m_pEntity;
		pEntity->SetLocalToWorld(mat);
		m_lEntities.push_back(pEntity);

		if (pEntity->GetType() == ENTITY_CAMERA)
		{
			m_lCameras.push_back(dynamic_cast<Camera*>(pEntity));
		}
		else if (pEntity->GetType() == ENTITY_LIGHT)
		{
			m_lLights.push_back(dynamic_cast<Light*>(pEntity));
		}
		else if (pEntity->GetType() == ENTITY_NORMAL)
		{
			if (pEntity->IsDrawable() && pEntity->IsVisible())
				m_lRenderObjects.push_back(pEntity->GetRenderObject());
		}
	}
	else if (pNode->m_bLeaf == false)
	{
		std::list<SceneNode*>::iterator it = pNode->m_lNodes.begin();
		while (it != pNode->m_lNodes.end())
		{
			SceneNode* pNode = *it;
			TraverseSceneGraph(pNode, mat * pNode->GetTransformation());
			it++;
		}
	}
}

} // namespace Scene
} // namespace Magnet