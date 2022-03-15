#pragma once
#include <Base\NE_PrivateCommon.h>

namespace NuclearEngine {
	namespace ECS {	class Scene;}
	namespace Managers { class AssetManager; }

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

			GameInfo *GetGameInfo();

			// Arranged function.
			virtual void Initialize();	 // At Startup
			virtual void Load();          // Loading Assets
			virtual void Update(float Deltatime);        // Render The Game
			virtual void Render(float Deltatime);        // Render The Game
			virtual void ExitRendering(); // Exit Rendering
			virtual void Shutdown();      // At Shutdown

			virtual void OnMouseScroll(double xpos, double ypos);
			virtual void OnMouseMovement(int xpos, int ypos);
			virtual void OnWindowResize(int width, int height);

			// Helper Functions
			virtual void LoadFinishUp();

			void SetActiveScene(ECS::Scene* scene);
			ECS::Scene* GetActiveScene();

			void SetDefaultAssetManager(Managers::AssetManager* assetmanager);
			Managers::AssetManager* GetDefaultAssetManager();

			float FPS, FrameTime, ClockTime;

			float DeltaTime = 0.0f;	// time between current frame and last frame
			float LastFrame = 0.0f;
		protected:
			GameInfo *mGameinfo;
			ECS::Scene* mScene;
			Managers::AssetManager* mAssetManager;

		};

	}
}