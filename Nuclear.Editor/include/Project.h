#pragma once
#include <Common.h>
#include <filesystem>


namespace Nuclear::Editor
{

	class Project
	{
	public:
		Project();

		Assets::Scene* GetActiveScene();

		void ShowProjectFolderView();

		void SetPath(const std::filesystem::path& path);

		Assets::Scene* AddNewScene();
	protected:
		Managers::AssetManager AssetLoader;
		Managers::SceneManager SceneMgr;

		std::filesystem::path mPath;
	};
}