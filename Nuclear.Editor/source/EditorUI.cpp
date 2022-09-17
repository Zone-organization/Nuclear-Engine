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
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			constexpr ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
			constexpr ImGuiWindowFlags   window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("MainDockSpace", nullptr, window_flags);
			ImGui::PopStyleVar(3);
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			ImGui::End();
		}

		RenderMainMenuBar();

		if (pActiveProject)
		{
			pActiveProject->ShowProjectFolderView();
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
					if (ImGui::MenuItem("Project"))
					{
						//TODO: Multiple Projects
						pActiveProject = &mEditorInstance->mActiveProject;
					}

					if (ImGui::MenuItem("Scene", NULL, false, (pActiveProject == nullptr) ? false : true))
					{
						pActiveProject->AddNewScene();
					}
					ImGui::Separator();

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
		auto scene = pActiveProject->GetActiveScene();

		if (scene)
		{
			ImGui::Begin("Entity Explorer");

			if (ImGui::TreeNode(scene->GetName().c_str()))
			{
				auto view = scene->GetRegistry().view<Components::EntityInfoComponent>();

				for (auto entity : view)
				{
					auto& Einfo = view.get<Components::EntityInfoComponent>(entity);
					auto index = (Uint32)entity;

					ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					static int selectedindex = 0;

					if (selectedindex == index)
					{
						node_flags |= ImGuiTreeNodeFlags_Selected;
						mEditor.Render(entity, scene->GetRegistry(), Einfo);
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

			if (ImGui::Button("Add Entity"))
			{
				auto entity = scene->CreateEntity();
			}
			ImGui::End();
		}
	}

}