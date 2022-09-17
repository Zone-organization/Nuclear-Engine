#pragma once
#include <Core\NE_Common.h>
#include <Engine\Assets\Scene.h>

namespace Nuclear {
	namespace Managers {
		class AssetManager; 
		class SceneManager;
	}

	enum class ClientType {
		Game,
		Editor,
		Test,
		Unknown
	};

	struct NEAPI ClientInfo
	{
		std::string mName;
		std::string mVersion;
		std::string mDeveloper;

		ClientType mType = ClientType::Game;
	};

	class NEAPI Client
	{
	public:
		Client(const ClientInfo& yourinfo = ClientInfo());
		~Client();

		ClientInfo GetClientInfo();

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

		void SetDefaultSceneManager(Managers::SceneManager* scenemanager);
		Managers::SceneManager* GetDefaultSceneManager();

		void SetDefaultAssetManager(Managers::AssetManager* assetmanager);
		Managers::AssetManager* GetDefaultAssetManager();

		float FPS, FrameTime, ClockTime;

		float DeltaTime = 0.0f;	// time between current frame and last frame
		float LastFrame = 0.0f;
	protected:
		ClientInfo minfo;
		Assets::Scene* pScene;
		Managers::SceneManager* mSceneManager;
		Managers::AssetManager* mAssetManager;
	};
}