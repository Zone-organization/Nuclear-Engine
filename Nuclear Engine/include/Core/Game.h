#pragma once
#include <NE_PrivateCommon.h>

namespace NuclearEngine {
	namespace Core {

		struct NEAPI GameInfo
		{
			const char* Name;
			const char* Version;
			const char* Developer;
		};

		class NEAPI Game
		{
		public:
			Game();
			Game(GameInfo *yourinfo);
			~Game();

			GameInfo *GetInfo();

			// Arranged function.
			virtual void Initialize();	 // At Startup
			virtual void Load();          // Loading Assets
			virtual void Update(float Deltatime);        // Render The Game
			virtual void Render(float Deltatime);        // Render The Game
			virtual void ExitRendering(); // Exit Rendering
			virtual void Shutdown();      // At Shutdown

			virtual void OnMouseMovement(float xpos, float ypos);
			virtual void OnWindowResize(int width, int height);

			float FPS, FrameTime;
		protected:
			GameInfo *gameinfo;
		};

	}
}