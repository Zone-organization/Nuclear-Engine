#pragma once
#include <Common.h>
#include <Project.h>
#include "EntityEditor.h"

class NuclearEditor;

namespace Nuclear::Editor {
	class AssetLibraryViewer
	{
	public:
		AssetLibraryViewer(NuclearEditor* editor);

		void SetProject(Project* project);
		void Render();

	protected:
		Assets::AssetLibrary* pAssetLibrary;

		Project* pActiveProject;
		NuclearEditor* mEditorInstance;
	};
}