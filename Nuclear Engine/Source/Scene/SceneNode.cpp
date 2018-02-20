#include <Scene\SceneNode.h>

namespace NuclearEngine
{
	static unsigned int CounterID = 0;

	SceneNode::SceneNode()
	{
		SceneNode(CounterID++);
	}
	SceneNode::SceneNode(unsigned int id)
	{
		m_ID = id;
	}
	SceneNode::~SceneNode()
	{
	}
	unsigned int SceneNode::GetID()
	{
		return m_ID;
	}
	void SceneNode::AddChild(SceneNode *node)
	{
		// check if this child already has a parent. If so, first remove this scene node from its 
		// current parent. Scene nodes aren't allowed to exist under multiple parents.
		if (node->m_Parent)
		{
			//node->m_Parent->RemoveChild(node->m_ID);
		}
		node->m_Parent = this;
		m_Children.push_back(node);
	}

	void SceneNode::RemoveChild(unsigned int id)
	{
		auto it = std::find(m_Children.begin(), m_Children.end(), GetChild(id));
		if (it != m_Children.end())
			m_Children.erase(it);
	}

	std::vector<SceneNode*> SceneNode::GetChildren()
	{
		return m_Children;
	}

	unsigned int SceneNode::GetChildCount()
	{
		return m_Children.size();
	}

	SceneNode *SceneNode::GetChild(unsigned int id)
	{
		for (unsigned int i = 0; i < m_Children.size(); ++i)
		{
			if (m_Children[i]->GetID() == id)
				return m_Children[i];
		}
		return nullptr;
	}

	SceneNode* SceneNode::GetChildByIndex(unsigned int index)
	{
		assert(index < GetChildCount());
		return m_Children[index];
	}

	SceneNode *SceneNode::GetParent()
	{
		return m_Parent;
	}

	void SceneNode::Update(float dt)
	{
		if (m_Parent)
		{
			m_transform.Update(m_Parent->m_transform.GetTransform());
		}
		else 
		{
			m_transform.Update();
		}
		for (unsigned int i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->Update(dt);
		}
	}

	void SceneNode::Render()
	{
		for (unsigned int i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->Render();
		}
	}

}