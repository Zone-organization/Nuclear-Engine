#pragma once
#include <Scene\SceneNode.h>
#include <Components\Model.h>

namespace NuclearEngine
{
	//GameObject is a scene node tailored for 3D games (containes model , materials , etc)
	class NEAPI GameObject : public SceneNode
	{
	public:
		GameObject(Components::Model *model = nullptr);
		void SetModel(Components::Model *model);
		void Render() override;
	private:
		Components::Model *m_model;
	};
}