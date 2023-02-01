#pragma once
#include <Common.h>
#include <filesystem>


namespace Nuclear::Editor
{

	struct ProjectInfo
	{
		std::string mProjectName;


	};

	class Project
	{
	public:
		Project();

		void Initalize(const ProjectInfo& info);

		ProjectInfo GetProjectInfo() const;

		//Assets::Scene* GetActiveScene();

		Assets::Material* GetDefaultMaterial();
		void SetDefaultMaterial(Assets::Material* mat);

		void ShowProjectFolderView();

		void SetPath(const std::filesystem::path& path);

		void ImportAssetWindow(const std::string& path);

		Assets::Scene* AddNewScene();
	protected:

		Assets::Material* mDefaultMaterial = nullptr;
		ProjectInfo mInfo;
		std::filesystem::path mPath;
	//	Core::Parsers::XMLDocument mProjectXMLFile;
	};
}