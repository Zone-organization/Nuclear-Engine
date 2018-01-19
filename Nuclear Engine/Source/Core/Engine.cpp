#include <Core\Engine.h>
#include <NE_Common.h>
#include <Core\Application.h>
#include <Platform\Input.h>
#include <..\Source\Tests\Test1.h>
#include <..\Source\Tests\Test2.h>
#include <..\Source\Tests\Test3.h>

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

		static Game * YourGame;

		static Engine::State _state;
		bool Engine::Initialize(std::wstring WindowTitle, unsigned int width, unsigned int height, bool Debug, bool DisableLog)
		{

			if (HasBeenInitialized != true)
			{
				Application::Create(WindowTitle, width, height, false);
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

				Log->Info("[Engine] Built On: ");

#ifdef 	_WIN32                           //Windows Platform 32 bit Macro
#define _BUILTONWINDOWS32_
				Log->Info("Windows 32 Bit\n");
#endif

#ifdef 	_WIN64                           //Windows Platform 64 bit Macro
#define _BUILTONWINDOWS64_
				Log->Info("Windows 64 bit\n");
#endif

#ifdef 	__gnu_linux__                  //Linux Platform Macro
#define _BUILTONLINUXOS_
				Log->Info("Linux Operating System\n");
#endif

#ifndef __gnu_linux__
#ifdef 		__linux__                     //Linux Kernel Macro
#define _BUILTONLINUXBASEDSYSTEM_
				Log->Info("Linux-Based Operating System\n");
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4067)
#endif

#ifndef _WIN32  || _WIN64  || _gnu_linux__ ||  __linux__
#define _BUILTONUNKNOWN_
				Log->Info("Unknown Operating System\n");
#endif

				Input::Mouse::SetInputMode(Input::Mouse::InputMode::Normal);

				HasBeenInitialized = true;
				return true;
			}
			else if (HasBeenInitialized == true) 
			{
				Log->FatalError("[Engine] Engine Has Been Already Initialized!\n");
				return false;
			}
			return true;
		}

		void Engine::ShutDown()
		{
			Log->Info("[Engine] Shutting Down Engine.\n");
			Application::Delete();
		}

		void Mousecb(double X, double Y)
		{
			YourGame->MouseMovementCallback(X, Y);
		}

		void Engine::Run(Game * _YourGame)
		{
			YourGame = _YourGame;
			Log->Info("[Engine] Running Game.\n");

			if (YourGame->GetInfo() != nullptr)
			{
				Log->Info(std::string("[Engine] Game Name: " + std::string(YourGame->GetInfo()->Name) + "\n"));
				Log->Info(std::string("[Engine] Game Version: " + std::string(YourGame->GetInfo()->Version) + "\n"));
				Log->Info(std::string("[Engine] Game Developer: " + std::string(YourGame->GetInfo()->Developer) + "\n"));
			}

			Application::SetMouseMovementCallback(Mousecb);

			Engine::Game_Initialize();
			Application::Display();
			Engine::Game_StartupLoad();
			Engine::Game_Load();
			Engine::Game_Render();
			Engine::Game_ExitRender();
			Engine::Game_Shutdown();
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
		}

		bool Engine::ShouldClose()
		{
			return Application::ShouldClose();
		}

		Game * Engine::GetGame()
		{
			return YourGame;
		}

		void Engine::Game_Initialize()
		{
			_state = Engine::State::Initializing;

			YourGame->Initialize();
		}

		void Engine::Game_StartupLoad()
		{
			_state = Engine::State::StartupLoad;
			YourGame->StartupLoad();
		}

		void Engine::Game_Load()
		{
			_state = Engine::State::Loading;

			YourGame->Load();
		}

		void Engine::Game_Render()
		{
			_state = Engine::State::Rendering;
			Log->Info("[Engine] Starting Game Rendering!\n");

			Platform::Clock clock;
			//Main Game Loop
			while (Core::Engine::ShouldClose() != true)
			{
				// per-frame time logic (ensure speed is constant through all platforms)
				// --------------------
				float currentFrame = clock.GetElapsedTime().AsSeconds();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				YourGame->PreRender(deltaTime);
				YourGame->Render();
				YourGame->PostRender();
			}
		}

		void Engine::Game_ExitRender()
		{
			_state = Engine::State::ExitingRendering;
			YourGame->ExitRendering();
		}

		void Engine::Game_Shutdown()
		{
			_state = Engine::State::Shuttingdown;
			YourGame->Shutdown();
		}

		void Engine::ProcessEvents()
		{
			return Application::ProcessEvents();
		}

		void Engine::SetWindowTitle(std::wstring WindowTitle)
		{
			return Application::SetTitle(WindowTitle);
		}
	}
}