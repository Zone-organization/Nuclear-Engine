#pragma once
#include <Core\Game.h>
#include <string>
namespace NuclearEngine {
	namespace Core {
		enum class NEAPI GameState { Initializing, StartupLoad, Loading, Rendering, ExitingRendering, Shuttingdown };


			class NEAPI Engine
			{
			public:

				static bool Initialize(std::wstring WindowTitle, unsigned int width, unsigned int height, bool Debug = true, bool DisableLog = false);

				static void ShutDown();

				static void Run(Game *YourGame);

				static void ProcessEvents();

				static void SetWindowTitle(std::wstring WindowTitle);
				
				static bool ShouldClose();

				static Game *GetGame();

				static void Game_Initialize();
				static void Game_StartupLoad();
				static void Game_Load();
				static void Game_Render();
				static void Game_ExitRender();
				static void Game_Shutdown();			
			};

		}
	}