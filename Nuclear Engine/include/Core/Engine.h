#pragma once
#include <Core\Game.h>
#include <string>
namespace NuclearEngine {
	namespace Core {
		

			class NEAPI Engine
			{
			public:
				enum class State { Initializing, Loading, Rendering, ExitingRendering, Shuttingdown };

				static bool Initialize(std::wstring WindowTitle, unsigned int width, unsigned int height, bool fullscreen = false, bool DisableLog = false);

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