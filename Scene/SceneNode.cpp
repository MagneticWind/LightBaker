#include "SceneNode.h"

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
SceneNode::SceneNode()
{
	m_bLeaf = false;
	m_pEntity = 0;
}

//------------------------------------------------------------------
SceneNode::~SceneNode()
{
}

//------------------------------------------------------------------
void SceneNode::AddChildNode(SceneNode* pChildNode)
{
	m_lNodes.push_back(pChildNode);
}

} // namespace Scene
} // namespace Magnet
