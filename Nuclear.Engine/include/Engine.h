#pragma once
#include <Core\Game.h>
#include <Core\Window.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\PhysX\PhysXEngine.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>

namespace Nuclear
{
	namespace Core
	{
		class Window;
		struct EngineStartupDesc
		{
			WindowDesc mEngineWindowDesc  = WindowDesc();
			Uint32 Samples = 2;
			RENDER_DEVICE_TYPE Renderer = RENDER_DEVICE_TYPE::RENDER_DEVICE_TYPE_D3D11;
			bool DebugRenderAPI = false;
			bool VSync = true;
			bool AutoSelectRenderer = true;

			//TODO: Move
			Graphics::GraphicsEngineDesc mGraphicsEngineDesc;
			PhysX::PhysXEngineDesc mPhysXEngineDesc;
			bool AutoInitAudioEngine = true;
			bool AutoInitPhysXEngine = true;

			//Enables some debugging options that slows down performance in runtime
			bool Debug = DEBUG_TRUE_BOOL;
		};

		class NEAPI Engine
		{
		public:
			//Engine(Engine const&) = delete;
			//void operator=(Engine const&) = delete;

			enum class State { Initializing, Loading, Rendering, ExitingRendering, Shuttingdown };

			bool Start(const EngineStartupDesc& desc);

			static Engine* GetInstance();

			void Shutdown();

			void BeginFrame();

			void EndFrame();

			Window* GetMainWindow();
			void SetGame(Game* YourGame);

			void LoadGame();
			void RunGame();
			void EndGame();

			void SetState(const State& state);

			bool ShouldClose();

			bool isDebug();

			Game *GetGame();

		private:
			//Engine() {}
			Game* GamePtr;
			Game Defaultgame;
			Core::Window MainWindow;

			Engine::State Engine_State;
			bool gisDebug = DEBUG_TRUE_BOOL;
			void Game_Loop_Render();
		};

	}
}