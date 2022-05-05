#pragma once
#include <Common.h>
#include <Project.h>

class NuclearEditor;
	
namespace Nuclear::Editor {
	class EditorUI
	{
	public:
		EditorUI(NuclearEditor* editor);

		void SetProject(Project* project);

		void Render();

	protected:
		void RenderMainMenuBar();

		Project* pActiveProject;
		NuclearEditor* mEditorInstance;

		ECS::Scene* mScene;
		ECS::Entity mSelectedEntity;

		void RenderEntityEditor(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo);
		void RenderEntityExplorer();

	};
}