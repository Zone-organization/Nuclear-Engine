#pragma once
#include <Common.h>
namespace Nuclear::Editor
{

	class Project
	{
	public:
		Project();

		ECS::Scene* GetActiveScene();

	//	ECS::Scene* AddNewScene();
	protected:
		Managers::AssetManager AssetLoader;

		ECS::Scene MainScene;


		//TODO
		//ECS::Scene* pActiveScene;
		//std::unordered_map<Uint32, ECS::Scene> mScenes;          //Should be moved
	};
}