#pragma once
#include <Core\Game.h>
namespace NuclearEngine {
	namespace Core {
		struct NEAPI EditorInfo
		{
			const char* Name = "Unknown";
			const char* Version = "00-00-00";
			const char* Developer = "Unknown";
		};
		class NEAPI Editor : public Game
		{
		public:
			Editor();
			Editor(EditorInfo *yourinfo);
			~Editor();

			Game *GetGame();
			EditorInfo *GetEditorInfo();

			// Arranged functions.
			virtual void Start();					 // At Startup
			virtual void Initialize(Game * mGame);	 // after Startup
			virtual void Load();				     // Loading Assetss
			virtual void Update(float Deltatime);    // Update The Editor "Before Rendering"
			virtual void Render(float Deltatime);    // Render The Editor
			virtual void ExitRendering();			 // Exit Rendering
			virtual void Shutdown();				 // At Shutdown

			virtual void OnMouseScroll(double xpos, double ypos);
			virtual void OnMouseMovement(double xpos, double ypos);
			virtual void OnWindowResize(int width, int height);
			float FPS, FrameTime, ClockTime;
		protected:
			Game * mGame;
			EditorInfo *mEditorInfo;
		};

	}
}
