#pragma once
#include <Common.h>
#include <filesystem>


namespace Nuclear::Editor
{

	class Project
	{
	public:
		Project();

		void Initalize();

		Assets::Scene* GetActiveScene();

		Assets::Material* GetDefaultMaterial();
		void SetDefaultMaterial(Assets::Material* mat);

		void ShowProjectFolderView();

		void SetPath(const std::filesystem::path& path);

		Assets::Scene* AddNewScene();
	protected:
		Managers::AssetManager AssetLoader;
		Managers::SceneManager SceneMgr;

		Assets::Material* mDefaultMaterial = nullptr;
		std::filesystem::path mPath;
	};
}