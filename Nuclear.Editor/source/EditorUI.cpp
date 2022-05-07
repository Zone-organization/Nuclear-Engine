#include "EditorUI.h"
#include "Nuclear.Editor.h"
#include <type_traits>
#include <Engine/ECS/entt/core/type_info.hpp>
using namespace entt::literals;

namespace Nuclear::Editor 
{

	EditorUI::EditorUI(NuclearEditor* editor)
	{
		mEditorInstance = editor;
		pActiveProject = nullptr;
	}
	void EditorUI::SetProject(Project* project)
	{
		pActiveProject = project;
	}
	void EditorUI::Render()
	{
		RenderMainMenuBar();

		if (mScene)
		{
			RenderEntityExplorer();
		}
	}

	void EditorUI::RenderMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			//Scene / Project
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New"))
				{
					if (pActiveProject != nullptr)
					{
						if (ImGui::MenuItem("Scene"))
						{
							//auto sceneptr = pActiveProject->AddNewScene();
							//if (mScene == nullptr) { mScene = sceneptr; }
						}
						ImGui::Separator();
					}

					if(ImGui::MenuItem("Project"))
					{
						pActiveProject = &mEditorInstance->mActiveProject;
						mScene = pActiveProject->GetActiveScene();
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void EditorUI::RenderEntityExplorer()
	{
		ImGui::Begin("Entity Explorer");
		if (ImGui::TreeNode(mScene->GetName().c_str()))
		{
			auto view = mScene->GetRegistry().view<Components::EntityInfoComponent>();

			for (auto entity : view)
			{
				auto& Einfo = view.get<Components::EntityInfoComponent>(entity);
				auto index = (Uint32)entity;

				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

				static int selectedindex = 0;

				if (selectedindex == index)
				{
					node_flags |= ImGuiTreeNodeFlags_Selected;
					mEditor.Render(entity, mScene->GetRegistry(), Einfo);
				}

				ImGui::PushID(index);


				ImGui::TreeNodeEx(Einfo.mName.c_str(), node_flags);
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					selectedindex = index;
				}

				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if(ImGui::Button("Add Entity"))
		{
			auto entity = mScene->CreateEntity();	
		}
		ImGui::End();
	}

}