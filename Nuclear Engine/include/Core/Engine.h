#pragma once
#include <Core\Game.h>
#include <Core\ApplicationDesc.h>
#include <LLGL/Input.h>
namespace NuclearEngine
{
	namespace Input
	{
		namespace Keyboard
		{
			using LLGL::Key;
		}
	}

	namespace Core
	{
		struct EngineStartupDesc
		{
			ApplicationDesc mAppdesc;


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