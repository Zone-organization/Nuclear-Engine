#pragma once
#include <Common.h>
#include <Project.h>
#include "EntityEditor.h"
#include "AssetLibraryViewer.h"

class NuclearEditor;

namespace Nuclear::Editor 
{
	class EditorUILayer
	{
	public:
		EditorUILayer(const char* pName) : m_pName(pName) { }

		virtual void Init() = 0;
		virtual void Update() = 0;

		const char* GetName() const { return m_pName; }


		void Enable(bool enable) { mEnabled = enable; }
		bool IsEnabled() const { return mEnabled; }

	protected:
		const char* m_pName = nullptr;
		bool mEnabled = true;
	};
}