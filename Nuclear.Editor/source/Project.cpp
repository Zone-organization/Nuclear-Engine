#include "Project.h"
#include "Core\Window.h"

namespace Nuclear::Editor
{
    Project::Project()
    {
        //TODO : create folder for the project
        mPath = std::filesystem::absolute(std::filesystem::current_path());
    }
	void Project::Initalize(const ProjectInfo& info)
	{
		mInfo = info;


		//Engine::GetInstance()->GetMainWindow()->SetTitle("Nuclear Engine - " + mInfo.mProjectName);

		//TODO
	//	mDefaultMaterial = &AssetLoader.DefaultGreyTex;
	}
	ProjectInfo Project::GetProjectInfo() const
	{
		return mInfo;
	}
	Assets::Scene* Project::GetActiveScene()
    {
        return SceneMgr.GetActiveScene();
    }
	Assets::Material* Project::GetDefaultMaterial()
	{
		return mDefaultMaterial;
	}
	void Project::SetDefaultMaterial(Assets::Material* mat)
	{
		mDefaultMaterial = mat;
	}

	Managers::AssetManager* Project::GetAssetManager()
	{
		return &AssetLoader;
	}

	std::string AssetTypeToString(Assets::AssetType type)
	{
		switch (type)
		{
		case Nuclear::Assets::AssetType::Unknown:
			return "Unknown";
		case Nuclear::Assets::AssetType::Scene:
			return "Scene";
		case Nuclear::Assets::AssetType::Image:
			return "Image";
		case Nuclear::Assets::AssetType::Mesh:
			return "Mesh";
		case Nuclear::Assets::AssetType::Material:
			return "Material";
		case Nuclear::Assets::AssetType::MaterialData:
			return "MaterialData";
		case Nuclear::Assets::AssetType::Animations:
			return "Animations";
		case Nuclear::Assets::AssetType::AudioClip:
			return "AudioClip";
		case Nuclear::Assets::AssetType::Font:
			return "Font";
		case Nuclear::Assets::AssetType::Shader:
			return "Shader";
		default:
			return "Unknown";
		}

		return "Unknown";
	}

	void Project::ImportAssetWindow(const std::string& path, const Assets::AssetType& type)
	{
		if (ImGui::Begin("Import Asset"))
		{
			if (type == Assets::AssetType::Image)
			{
				Importers::ImageLoadingDesc desc;

				AssetLoader.Import(path, desc);
			}

			ImGui::End();
		}
		/*if (ImGui::Button(("Load: %s", AssetTypeToString(type).c_str())))
		{
			assert(false);
		}*/
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
						auto type = AssetLoader.GetAssetType(entryName.c_str());


						if (type != Nuclear::Assets::AssetType::Unknown)
						{
							if (type == Assets::AssetType::Image)
							{
								Importers::ImageLoadingDesc desc;

								AssetLoader.Import(path.string(), desc);
							}
							else if (type == Assets::AssetType::Shader)
							{
								Importers::ShaderLoadingDesc desc;

								AssetLoader.Import(path.string(), desc);
							}
							else if (type == Assets::AssetType::Mesh)
							{
								Importers::MeshLoadingDesc desc;
								AssetLoader.Import(path.string(), desc);
							}
						}



						/*if (mOnPicked)
							mOnPicked(path);
						Hide();*/
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

		/*auto EController = result->CreateEntity();
		EController.AddComponent<Components::CameraComponent>(&Camera);
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));

		result->GetSystemManager().Add<Systems::DebugSystem>();
		result->GetSystemManager().Add<Systems::RenderSystem>();
		result->GetSystemManager().Add<Systems::CameraSystem>(&Camera);
		result->GetSystemManager().Add<Systems::LightingSystem>();*/

		return result;
	}

    //ECS::Scene* Project::AddNewScene()
    //{
    //  //  mScenes.push_back(ECS::Scene());
    //   // return &mScenes.back();
    //    return nullptr;

    //}
}