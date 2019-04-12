#pragma once
#include <Core\Game.h>
#include <Core\ApplicationDesc.h>
#include <Engine\Graphics\GraphicsEngine.h>

namespace NuclearEngine
{

	namespace Core
	{
		struct EngineStartupDesc
		{
			ApplicationDesc mAppdesc;
			Graphics::GraphicsEngineDesc mGraphicsEngineDesc;
			bool InitAudioEngine = true;
		};

		class NEAPI Engine
		{
		public:
			enum class State { Initializing, Loading, Rendering, ExitingRendering, Shuttingdown };

			static bool Start(const EngineStartupDesc& desc);

			static void Shutdown();

			static void RunGame(Game *YourGame);

			static void SetState(const State& state);

			static bool ShouldClose();

			static Game *GetGame();

		private:
			static void Game_Loop_Render();
		};

	}
}