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
		pAssetLibrary = &project->GetAssetManager()->mLibrary;

	}
	std::string SplitFilename(const std::string& str)
	{
		std::size_t found = str.find_last_of("/");
		return str.substr(found + 1);
	}

	void ImageViewer(Assets::Image* img)
	{
		ImGui::Begin("Asset Info");

		ImVec2 tex_sz(128.f, 128.f);
		ImGui::Text(SplitFilename(img->GetName()).c_str());

		ImGui::Separator();
		ImGui::Image(img->mTextureView, tex_sz);

		ImGui::Text("Path: %s", img->GetName().c_str());
		ImGui::Text("Width: %i", img->mData.mWidth, " - Height: %i", img->mData.mHeight);


		ImGui::End();
	}
	void AssetLibraryViewer::Render()
	{
		ImGui::Begin("Assets Library Viewer");
		ImVec2 tex_sz(128.f, 128.f);

		if (ImGui::BeginTabBar("Asset Library"))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			if (ImGui::BeginTabItem("Images"))
			{
				static int count = 0;
				static Assets::Image* img = nullptr;
				for (auto& i : pAssetLibrary->mImportedImages.mData)
				{
					ImGui::PushID(count);
					if (i.second.mTextureView)
					{
						ImGui::BeginGroup();
						{
							ImGui::Image(i.second.mTextureView, tex_sz);
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
					if (count + 1 < pAssetLibrary->mImportedImages.mData.size() && next_button_x2 < window_visible_x2)
						ImGui::SameLine();
					ImGui::PopID();

					count++;
				}
				count = 0;
				ImGui::EndTabItem();

				if (img)
				{
					if (img->isValid)
					{
						ImageViewer(img);
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
}