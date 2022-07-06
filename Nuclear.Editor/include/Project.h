#pragma once
#include <Common.h>
#include <filesystem>


namespace Nuclear::Editor
{

	class Project
	{
	public:
		Project();

		ECS::Scene* GetActiveScene();

		void ShowProjectFolderView();

		void SetPath(const std::filesystem::path& path);

	//	ECS::Scene* AddNewScene();
	protected:
		Managers::AssetManager AssetLoader;

		ECS::Scene MainScene;

		std::filesystem::path mPath;

		//TODO
		//ECS::Scene* pActiveScene;
		//std::unordered_map<Uint32, ECS::Scene> mScenes;          //Should be moved
	};
}