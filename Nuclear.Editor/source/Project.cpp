#include "Project.h"

namespace Nuclear::Editor
{
    Project::Project()
    {
        //TODO : create folder for the project
        mPath = std::filesystem::absolute(std::filesystem::current_path());
    }
	Assets::Scene* Project::GetActiveScene()
    {
        return SceneMgr.GetActiveScene();
    }
    void Project::ShowProjectFolderView()
    {
		if (ImGui::Begin("FilePicker"))
		{
			// Show the current path with clickable button for each folder to travel through
			ImGui::Text("Current path : ");
			std::filesystem::path fullpath;
			for (const std::filesystem::path& path : mPath)
			{
				fullpath /= path;
				ImGui::SameLine();
				if (ImGui::Button(path.string().c_str()))
				{
					SetPath(fullpath);
					break;
				}
			}

			auto ButtonNoIcon = [](const std::string& name)
			{
				ImGui::PushID(name.c_str());
				bool clicked = false;
				clicked |= ImGui::Button(name.c_str());
				ImGui::PopID();
				return clicked;
			};

			//auto ButtonWithIcon = [](const std::string& name, const sf::Texture& icon)
			//{
			//	ImGui::PushID(name.c_str());
			//	bool clicked = false;
			//	if (icon.getSize().x && icon.getSize().y)
			//	{
			//		const ImTextureID imTex = ImGuiSFML::GLHandleToImTexture(icon.getNativeHandle());
			//		clicked |= ImGui::ImageButton(imTex, ImVec2(16, 16));
			//		ImGui::SameLine();
			//	}
			//	clicked |= ImGui::Button(name.c_str());
			//	ImGui::PopID();
			//	return clicked;
			//};
			auto ButtonFile = [&, this](const std::string& name) { return ButtonNoIcon(name); };
			auto ButtonFolder = [&, this](const std::string& name) { return ButtonNoIcon(name); };
			//auto IsMatchingFilter = [this](const std::string& extension) { return mSettings.extensionsFilter.empty() || std::any_of(mSettings.extensionsFilter.begin(), mSettings.extensionsFilter.end(), [&](const std::string& ext) { return ext == extension; }); };

			if (mPath.has_parent_path())
			{
				const std::filesystem::path parent = mPath.parent_path();
				if (ButtonFolder(".."))
				{
					SetPath(parent);
				}
			}

			while (!std::filesystem::exists(mPath))
				mPath = mPath.parent_path();

			for (const auto& entry : std::filesystem::directory_iterator(mPath))
			{
				const std::filesystem::path& path = entry.path();
				const std::string entryName = std::filesystem::relative(path, mPath).string();
				if (entry.is_directory())
				{
					if (ButtonFolder(entryName))
					{
						SetPath(path);
						break;
					}
				}
				else if (entry.is_regular_file())
				{

							if (ButtonFile(entryName.c_str()))
							{
								//if (mOnPicked)
								//	mOnPicked(path);
								//Hide();
							}
						
					
				}
				else
				{
					ImGui::Text(entryName.c_str());
				}
			}
		
		}
		ImGui::End();
    }
	
	void Project::SetPath(const std::filesystem::path& path)
	{
		mPath = path;
		// Remove ending / if any
		if (mPath.wstring().back() == std::filesystem::path::preferred_separator)
			mPath = mPath.parent_path();
	}

	Assets::Scene* Project::AddNewScene()
	{
		auto hashedname = Utilities::Hash("UnNamed Scene");

		AssetLoader.mLibrary.mImportedScenes.mData[hashedname] = Assets::Scene();
		auto result = &AssetLoader.mLibrary.mImportedScenes.mData[hashedname];


		//TODO
		result->SetName("UnNamed Scene");
		SceneMgr.CreateScene(result, (SceneMgr.GetActiveScene() == nullptr) ? true : false);

		return result;
	}

    //ECS::Scene* Project::AddNewScene()
    //{
    //  //  mScenes.push_back(ECS::Scene());
    //   // return &mScenes.back();
    //    return nullptr;

    //}
}