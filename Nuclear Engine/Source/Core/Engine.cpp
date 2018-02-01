#include <Core\Engine.h>
#include <NE_Common.h>
#include <Platform\Input.h>
#include <..\Source\Tests\Test1.h>
#include <..\Source\Tests\Test2.h>
#include <..\Source\Tests\Test3.h>
#include <..\Source\Tests\Test4.h>

/*
	      .-.               
          |N|          
          |U|
          |C|  
         _|L|_     
        | |E| |-.    
       /|  A  ` |     
      | |  R    |      
      |         | 
      \         /       
       |       |  
       |ZLIXINE| 
*/
namespace NuclearEngine {
	using namespace Platform;

	namespace Core {

		static bool HasBeenInitialized;

		static std::string MajorVersion = "0";
		static std::string MinorVersion = "001";

		// timing
		static float deltaTime = 0.0f;	// time between current frame and last frame
		static float lastFrame = 0.0f;

		static Game * GamePtr;
		static Game Defaultgame;

		static Engine::State Engine_State;
		bool Engine::Initialize(const ApplicationDesc& windowdesc)
		{

			if (HasBeenInitialized != true)
			{
				Log->Info("-------------------------- -Nuclear Engine- --------------------------\n");
				Log->Info("[Engine] Starting Engine...\n");
				Log->Info("[Engine] Version: ");
				Log->Info(MajorVersion);
				Log->Info(".");
				Log->Info(MinorVersion);
				Log->Info("\n[Engine] Built On: ");
				Log->Info(__DATE__);
				Log->Info("  At: ");
				Log->Info(__TIME__);
				Log->EndLine();

				Log->Info("[Engine] Built For: ");

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_32BIT
				Log->Info("Windows-PC 32 Bit\n");
#endif

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_64BIT
				Log->Info("Windows-PC 64 bit\n");
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4067)
#endif
				Log->Info("[Engine] Built With Renderer: ");


#ifdef 	NE_USE_CORE_OPENGL
				Log->Info("OpenGL 3.3 Core\n");
#endif

#ifdef 	NE_USE_DIRECTX11
				Log->Info("DirectX 11\n");
#endif

#ifdef 	NE_USE_RUNTIME_RENDER_API
				Log->Info("OpenGL 3.3 Core & DirectX 11\n");
#endif

				Application::Create(windowdesc);
				Application::SetMouseInputMode(MouseInputMode::Normal);

				HasBeenInitialized = true;
				return true;
			}
			else if (HasBeenInitialized == true) 
			{
				Log->FatalError("[Engine] Engine Has Been Already Initialized!\n");
				return false;
			}
			GamePtr = &Defaultgame;
			return true;
		}

		void Engine::ShutDown()
		{
			Log->Info("[Engine] Shutting Down Engine.\n");
			GamePtr = &Defaultgame;
			Core::Application::Shutdown();
		}

		void Engine::Run(Game * _YourGame)
		{
			GamePtr = _YourGame;
			Log->Info("[Engine] Running Game.\n");

			if (GamePtr->GetInfo() != nullptr)
			{
				Log->Info(std::string("[Engine] Game Name: " + std::string(GamePtr->GetInfo()->Name) + "\n"));
				Log->Info(std::string("[Engine] Game Version: " + std::string(GamePtr->GetInfo()->Version) + "\n"));
				Log->Info(std::string("[Engine] Game Developer: " + std::string(GamePtr->GetInfo()->Developer) + "\n"));
			}

			SetState(Engine::State::Initializing);
			GamePtr->Initialize();
			SetState(Engine::State::Loading);
			GamePtr->Load();			
			Engine::Game_Loop_Render();
			SetState(Engine::State::ExitingRendering);
			GamePtr->ExitRendering();
			SetState(Engine::State::Shuttingdown);
			GamePtr->Shutdown();
		}

		void Engine::Run(unsigned int TestNumber)
		{
			if (TestNumber == 1)
			{
				Test1 test;
				Engine::Run(&test);
			}
			else if (TestNumber == 2)
			{
				Test2 test;
				Engine::Run(&test);
			}
			else if (TestNumber == 3)
			{
				Test3 test;
				Engine::Run(&test);
			}
			else if (TestNumber == 4)
			{
				Test4 test;
				Engine::Run(&test);
			}
			else {
				Test3 test;
				Engine::Run(&test);
			}
		}

		bool Engine::ShouldClose()
		{
			return Core::Application::ShouldClose();
		}

		Game * Engine::GetGame()
		{
			return GamePtr;
		}

		void Engine::Game_Loop_Render()
		{
			SetState(Engine::State::Rendering);

			Platform::Clock clock;
			//Main Game Loop
			while (Core::Engine::ShouldClose() != true)
			{
				// per-frame time logic (ensure speed is constant through all platforms)
				float currentFrame = clock.GetElapsedTime().AsSeconds();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				GamePtr->FrameTime = 1000.0f / deltaTime;
				GamePtr->FPS = 1.0f / deltaTime;

				GamePtr->Update(deltaTime);
				GamePtr->Render(deltaTime);
			}
		}

		void Engine::SetState(const State & state)
		{
			Engine_State = state;
			std::string name;
			switch (Engine_State)
			{
				case Engine::State::Initializing:
					name = "Initializing";
					break;
				case Engine::State::Loading:
					name = "Loading";
					break;
				case Engine::State::Rendering:
					name = "Rendering";
					break;
				case Engine::State::ExitingRendering:
					name = "Exiting Rendering";
					break;
				case Engine::State::Shuttingdown:
					name = "Shutting down";
					break;
				default:
					name = "Unknown";
					break;
			}

			Log->Info("[Engine] Game state changed to " + name + "\n");
		}
	}
}