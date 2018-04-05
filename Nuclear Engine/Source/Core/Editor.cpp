#include "Core\Editor.h"


namespace NuclearEngine {
	namespace Core {
		Editor::Editor()
		{
			mEditorInfo = nullptr;
		}
		Editor::Editor(EditorInfo * yourinfo)
		{
			mEditorInfo = yourinfo;
		}
		Editor::~Editor()
		{
		}
		Game * Editor::GetGame()
		{
			return mGame;
		}
		EditorInfo * Editor::GetEditorInfo()
		{
			return mEditorInfo;
		}
		void Editor::Start()
		{
		}
		void Editor::Initialize(Game* game)
		{
		}

		void Editor::Load()
		{
		}
		void Editor::Update(float Deltatime)
		{
		}

		void Editor::Render(float Deltatime)
		{
		}
		void Editor::ExitRendering()
		{
		}
		void Editor::Shutdown()
		{
		}
		void Editor::OnMouseScroll(double val1, double val2)
		{
		}
		void Editor::OnMouseMovement(double xpos, double ypos)
		{
		}

		void Editor::OnWindowResize(int width, int height)
		{
		}

	}
}