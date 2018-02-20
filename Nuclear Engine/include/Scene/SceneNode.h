#pragma once
#include <NE_Common.h>
#include <vector>
#include <Components\Transform.h>
namespace NuclearEngine
{
	//Great thanks and cheer to cell engine, and joey de vris!
	//we use scene graph since it is easier to implement, and cell engine have it covered, we may consider entity later...
	class NEAPI SceneNode
	{
	public:
		SceneNode();
		SceneNode(unsigned int id);
		~SceneNode();

		unsigned int GetID();
		void AddChild(SceneNode *node);
		void RemoveChild(unsigned int id); // TODO(Joey): think of proper way to unqiuely idetnfiy child nodes (w/ incrementing node ID or stringed hash ID?)
		std::vector<SceneNode*> GetChildren();
		unsigned int  GetChildCount();
		SceneNode *GetChild(unsigned int id);
		SceneNode *GetChildByIndex(unsigned int index);
		SceneNode *GetParent();

		virtual void Update(float dt);
		virtual void Render();
	private:
		std::vector<SceneNode*> m_Children;
		SceneNode *m_Parent;
		Components::Transform m_transform;
		// each node is uniquely identified by a 32-bit incrementing unsigned integer
		unsigned int m_ID;
	};
}