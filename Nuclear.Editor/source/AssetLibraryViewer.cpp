#include "AssetLibraryViewer.h"
#include "FileDialog.h"
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

		mPath = Assets::AssetLibrary::Get().GetPath();
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
		if (ImGui::Begin("Assets Library"))
		{
			ImVec2 tex_sz(128.f, 128.f);

			if (ImGui::BeginTabBar("Asset Library"))
			{
				ImGuiStyle& style = ImGui::GetStyle();
				float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

				static char* file_dialog_buffer = nullptr;
				static char path[500] = "";

				ImGui::PushID("imprtbttn");
				ImGui::PushStyleColor(ImGuiCol_Tab, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_TabHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_TabActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.8f));
				if (ImGui::TabItemButton("Import"))
				{

					file_dialog_buffer = path;
					FileDialog::file_dialog_open = true;
				}
				ImGui::PopStyleColor(3);
				ImGui::PopID();

				if (ImGui::BeginTabItem("Textures"))
				{
					static int count = 0;
					static Assets::Texture* img = nullptr;
					for (auto& i : Assets::AssetLibrary::Get().mImportedTextures.mData)
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
						if (count + 1 < Assets::AssetLibrary::Get().mImportedTextures.mData.size() && next_button_x2 < window_visible_x2)
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

				if (FileDialog::file_dialog_open)
				{
					FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, file_dialog_buffer, sizeof(file_dialog_buffer), FileDialog::FileDialogType::OpenFile);
					if (!FileDialog::file_dialog_open)
					{
						Assets::AssetManager::Get().Import(path);
					}
				}
			}



			/*static char* file_dialog_buffer = nullptr;
			static char path[500] = "";

			ImGui::TextUnformatted("Path: ");
			ImGui::InputText("##path", path, sizeof(path));
			ImGui::SameLine();
			if (ImGui::Button("Browse##path")) {
				file_dialog_buffer = path;
				FileDialog::file_dialog_open = true;
				FileDialog::file_dialog_open_type = FileDialog::FileDialogType::SelectFolder;
			}

			if (FileDialog::file_dialog_open) {
				FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, file_dialog_buffer, sizeof(file_dialog_buffer), FileDialog::file_dialog_open_type);
			}*/
			ImGui::End();
		}
	}
}