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

		void SetPath(const std::filesystem::path& path);
	protected:
		Project* pActiveProject;
		NuclearEditor* mEditorInstance;
		std::filesystem::path mPath;
	};
}