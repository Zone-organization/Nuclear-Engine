#pragma once
#include <Core\Game.h>
#include <Core\Application.h>
namespace NuclearEngine
{
	namespace Core
	{

		class NEAPI Engine
		{
		public:
			enum class State { Initializing, Loading, Rendering, ExitingRendering, Shuttingdown };

			static bool Initialize(const ApplicationDesc& windowdesc);

			static void ShutDown();

			static void Run(Game *YourGame);

			static void Run(unsigned int TestNumber);

			static void SetState(const State& state);

			static bool ShouldClose();

			static Game *GetGame();

		private:
			static void Game_Loop_Render();
		};

	}
}