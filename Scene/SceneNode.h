#ifndef SCENENODE_H
#define SCENENODE_H

#include <list>

#include "Math\Matrix4f.h"
#include "Math\AABB.h"

namespace Magnet
{
namespace Scene
{
class IEntity;

class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	void AddChildNode(SceneNode* pChildNode);
	bool IsLeafNode() const;
	void SetLeaf(bool bLeaf);
	void SetEntity(IEntity* pEntity);
	// traverse subtree to compute bbox
	void UpdateBBox();
	const Math::AABB& GetBBox() const;

	const Math::Matrix4f& GetTransformation() const;

public:
	bool m_bLeaf;
	IEntity* m_pEntity;                   // only the leaf node will store the entity pointer
	Math::Matrix4f m_mTransform;          // concatenated transformation matrix for current node
	Math::AABB m_BBox;
	std::list<SceneNode*> m_lNodes;
};

//------------------------------------------------------------------
inline bool SceneNode::IsLeafNode() const
{
	return m_bLeaf;
}

//------------------------------------------------------------------
inline void SceneNode::SetLeaf(bool bLeaf)
{
	m_bLeaf = bLeaf;
}

//------------------------------------------------------------------
inline void SceneNode::SetEntity(IEntity* pEntity)
{
	m_pEntity = pEntity;
}

//------------------------------------------------------------------
inline const Math::AABB& SceneNode::GetBBox() const
{
	return m_BBox;
}

//------------------------------------------------------------------
inline const Math::Matrix4f& SceneNode::GetTransformation() const
{
	return m_mTransform;
}

} // namespace World
} // namespace Magnet




#endif