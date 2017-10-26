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
			virtual void StartupLoad();   // Called After Initializing can be used for GUI and Main Menu of game
			virtual void Load();          // Loading Assets
			virtual void PreRender(float tick_deltatime);		  // Update Some Values Before Rendering
			virtual void PostRender();        // Update Some Values After Rendering
			virtual void Render();        // Render The Game
			virtual void ExitRendering(); // Exit Rendering
			virtual void Shutdown();      // At Shutdown

			virtual void MouseMovementCallback(double xpos, double ypos);
		protected:
			GameInfo *gameinfo;
		};

	}
}