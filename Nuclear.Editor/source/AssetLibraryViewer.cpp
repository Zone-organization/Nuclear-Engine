#include "AssetLibraryViewer.h"
namespace Nuclear::Editor
{
	AssetLibraryViewer::AssetLibraryViewer(NuclearEditor* editor)
	{
		mEditorInstance = editor;
		pActiveProject = nullptr;
	}
	void AssetLibraryViewer::SetProject(Project* project)
	{
		pActiveProject = project;

		mPath = Assets::AssetLibrary::GetInstance().GetPath();
	}
	std::string SplitFilename(const std::string& str)
	{
		std::size_t found = str.find_last_of("/");
		return str.substr(found + 1);
	}

	void TextureViewer(Assets::Texture* img)
	{
		ImGui::Begin("Asset Info");

		ImVec2 tex_sz(128.f, 128.f);
		ImGui::Text(SplitFilename(img->GetName()).c_str());

		ImGui::Separator();
		ImGui::Image(img->GetTextureView(), tex_sz);

		ImGui::Text("Path: %s", img->GetName().c_str());
		ImGui::Text("Width: %i", img->GetWidth(), " - Height: %i", img->GetHeight());


		ImGui::End();
	}

	void AssetLibraryViewer::SetPath(const std::filesystem::path& path)
	{
		mPath = path;
		// Remove ending / if any
		if (mPath.wstring().back() == std::filesystem::path::preferred_separator)
			mPath = mPath.parent_path();
	}

	void AssetLibraryViewer::Render()
	{
		if(ImGui::Begin("Assets Library"))
		{
			ImVec2 tex_sz(128.f, 128.f);

			if (ImGui::BeginTabBar("Asset Library"))
			{
				ImGuiStyle& style = ImGui::GetStyle();
				float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

				if (ImGui::BeginTabItem("Textures"))
				{
					static int count = 0;
					static Assets::Texture* img = nullptr;
					for (auto& i : Assets::AssetLibrary::GetInstance().mImportedTextures.mData)
					{
						ImGui::PushID(count);
						if (i.second.GetTextureView())
						{
							ImGui::BeginGroup();
							{
								ImGui::Image(i.second.GetTextureView(), tex_sz);
								ImGui::Text(SplitFilename(i.second.GetName()).c_str());
								ImGui::EndGroup();
								if (ImGui::IsItemClicked())
									img = &i.second;
							}
						}
						else
						{
							//ERROR
						}

						float last_button_x2 = ImGui::GetItemRectMax().x;
						float next_button_x2 = last_button_x2 + style.ItemSpacing.x + tex_sz.x; // Expected position if next button was on same line
						if (count + 1 < Assets::AssetLibrary::GetInstance().mImportedTextures.mData.size() && next_button_x2 < window_visible_x2)
							ImGui::SameLine();
						ImGui::PopID();

						count++;
					}
					count = 0;
					ImGui::EndTabItem();

					if (img)
					{
						if (img->GetState() == Assets::IAsset::State::Created)
						{
							TextureViewer(img);
						}
					}
				}
				if (ImGui::BeginTabItem("Meshes"))
				{
					//Should show mesh icon only
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Materials"))
				{
					//Should show material rendered on a sphere
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			ImGui::End();
		}

		//if (ImGui::Begin("Assets Library Folder"))
		//{
		//	// Show the current path with clickable button for each folder to travel through
		//	std::filesystem::path fullpath;

		//	for (const std::filesystem::path& path : mPath)
		//	{
		//		//fullpath /= path;
		//		ImGui::SameLine();
		//		if (ImGui::Button(path.string().c_str()))
		//		{
		//			//SetPath(fullpath);
		//			break;
		//		}
		//	}

		//	auto ButtonNoIcon = [](const std::string& name)
		//	{
		//		ImGui::PushID(name.c_str());
		//		bool clicked = false;
		//		clicked |= ImGui::Button(name.c_str());
		//		ImGui::PopID();
		//		return clicked;
		//	};

		//	//auto ButtonWithIcon = [](const std::string& name, const sf::Texture& icon)
		//	//{
		//	//	ImGui::PushID(name.c_str());
		//	//	bool clicked = false;
		//	//	if (icon.getSize().x && icon.getSize().y)
		//	//	{
		//	//		const ImTextureID imTex = ImGuiSFML::GLHandleToImTexture(icon.getNativeHandle());
		//	//		clicked |= ImGui::ImageButton(imTex, ImVec2(16, 16));
		//	//		ImGui::SameLine();
		//	//	}
		//	//	clicked |= ImGui::Button(name.c_str());
		//	//	ImGui::PopID();
		//	//	return clicked;
		//	//};
		//	auto ButtonFile = [&, this](const std::string& name) { return ButtonNoIcon(name); };
		//	auto ButtonFolder = [&, this](const std::string& name) { return ButtonNoIcon(name); };
		//	//auto IsMatchingFilter = [this](const std::string& extension) { return mSettings.extensionsFilter.empty() || std::any_of(mSettings.extensionsFilter.begin(), mSettings.extensionsFilter.end(), [&](const std::string& ext) { return ext == extension; }); };

		//	if (mPath.has_parent_path())
		//	{
		//		const std::filesystem::path parent = mPath.parent_path();
		//		if (ButtonFolder(".."))
		//		{
		//			SetPath(parent);
		//		}
		//	}

		//	while (!std::filesystem::exists(mPath))
		//		mPath = mPath.parent_path();

		//	for (const auto& entry : std::filesystem::directory_iterator(mPath))
		//	{
		//		const std::filesystem::path& path = entry.path();
		//		const std::string entryName = std::filesystem::relative(path, mPath).string();
		//		if (entry.is_directory())
		//		{
		//			if (ButtonFolder(entryName))
		//			{
		//				SetPath(path);
		//				break;
		//			}
		//		}
		//		else if (entry.is_regular_file())
		//		{

		//			if (ButtonFile(entryName.c_str()))
		//			{
		//				/*	auto type = AssetLoader.GetAssetType(entryName.c_str());


		//					if (type != Nuclear::Assets::AssetType::Unknown)
		//					{
		//						if (type == Assets::AssetType::Image)
		//						{
		//							Importers::ImageLoadingDesc desc;

		//							AssetLoader.Import(path.string(), desc);
		//						}
		//						else if (type == Assets::AssetType::Shader)
		//						{
		//							Importers::ShaderLoadingDesc desc;

		//							AssetLoader.Import(path.string(), desc);
		//						}
		//						else if (type == Assets::AssetType::Mesh)
		//						{
		//							Importers::MeshLoadingDesc desc;
		//							AssetLoader.Import(path.string(), desc);
		//						}
		//					}*/



		//					/*if (mOnPicked)
		//						mOnPicked(path);
		//					Hide();*/
		//			}


		//		}
		//		else
		//		{
		//			ImGui::Text(entryName.c_str());
		//		}
		//	}
		//	ImGui::End();
		//}
	}
}