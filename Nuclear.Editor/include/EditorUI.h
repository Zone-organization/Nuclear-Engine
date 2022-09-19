#pragma once
#include <Common.h>
#include <Project.h>
#include "EntityEditor.h"

class NuclearEditor;
	
namespace Nuclear::Editor {
	class EditorUI
	{
	public:
		EditorUI(NuclearEditor* editor);

		void SetProject(Project* project);
		void NewProjectWindow();
		void Render();

	protected:
		void RenderMainMenuBar();
		EntityEditor mEditor;

		Project* pActiveProject;
		NuclearEditor* mEditorInstance;

		ECS::Entity mSelectedEntity;

		void RenderEntityExplorer();
		bool mNewPrjWindowOpen = false;
	};
}