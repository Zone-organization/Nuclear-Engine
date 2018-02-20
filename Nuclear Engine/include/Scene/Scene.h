#pragma once
#include <NE_Common.h>
#include <Scene\SceneNode.h>
namespace NuclearEngine
{
	//Scene is a global special scene node (called root) which encapsulates special nodes (as background)
	class NEAPI Scene
	{
	public:
		void Clear();
		SceneNode *GetRoot();

		void Update(float dt);
		void Render();
	private:
		SceneNode *Root;
	};
}