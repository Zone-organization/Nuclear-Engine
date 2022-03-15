#pragma once
#include <Core\Game.h>
#include <Core\ApplicationDesc.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\PhysX\PhysXEngine.h>

namespace NuclearEngine
{

	namespace Core
	{
		struct EngineStartupDesc
		{
			ApplicationDesc mAppdesc;
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
			enum class State { Initializing, Loading, Rendering, ExitingRendering, Shuttingdown };

			static bool Start(const EngineStartupDesc& desc);

			static void Shutdown();

			static void BeginFrame();

			static void EndFrame();

			static void SetGame(Game* YourGame);

			static void LoadGame();
			static void RunGame();
			static void EndGame();

			static void SetState(const State& state);

			static bool ShouldClose();

			static bool isDebug();

			static Game *GetGame();

		private:
			static void Game_Loop_Render();
		};

	}
}