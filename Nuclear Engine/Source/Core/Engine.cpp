#include <Core\Engine.h>
#include <NE_Common.h>
#include <Platform\Input.h>
#include <..\Source\Tests\ECSTests.h>
#include <..\Source\Tests\PhysXTests.h>
#include <..\Source\Tests\FMODTests.h>
#include <..\Source\Tests\DirectX11Tests.h>
#include <..\Source\Tests\OpenGLTests.h>
#include <..\Source\Tests\Test1.h>
#include <..\Source\Tests\Test2.h>
#include <..\Source\Tests\Test3.h>
#include <..\Source\Tests\Test4.h>
#include <..\Source\Tests\Test5.h>
#include <GUI\imgui.h>
#include "..\Core\imgui_impl\imgui_impl.h"
#include <Graphics\GraphicsEngine.h>
#include <Audio\AudioEngine.h>
#include <FMOD\includer.h> 

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

		static std::string MajorVersion = "0";
		static std::string MinorVersion = "001";

		// timing
		static float deltaTime = 0.0f;	// time between current frame and last frame
		static float lastFrame = 0.0f;

		static Game * GamePtr;
		static Game Defaultgame;

		static Engine::State Engine_State;

		void PrintIntroLog();

		bool Engine::Start(const EngineStartupDesc& desc)
		{
			PrintIntroLog();

			//Create platform specific app (window)
			Application::Create(desc.mAppdesc);
			Application::SetMouseInputMode(MouseInputMode::Normal);

			//init sub-engines
			Graphics::GraphicsEngine::Initialize();

			if(desc.InitAudioEngine)
				Audio::AudioEngine::Initialize();

			GamePtr = &Defaultgame;
			return true;
		}

		void Engine::Shutdown()
		{
			Log.Info("[Engine] Shutting Down Engine.\n");

			GamePtr = &Defaultgame;
			Core::Application::Shutdown();
			Audio::AudioEngine::Shutdown();
		}

		void Engine::RunGame(Game * _YourGame)
		{
			GamePtr = _YourGame;
			Log.Info("[Engine] Running Game.\n");

			if (GamePtr->GetGameInfo() != nullptr)
			{
				Log.Info(std::string("[Engine] Game Name: " + std::string(GamePtr->GetGameInfo()->Name) + "\n"));
				Log.Info(std::string("[Engine] Game Version: " + std::string(GamePtr->GetGameInfo()->Version) + "\n"));
				Log.Info(std::string("[Engine] Game Developer: " + std::string(GamePtr->GetGameInfo()->Developer) + "\n"));
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
		/*
		Normal Tests:
			From 1 to 10
		Special Tests:
			DirectX 11:  11
			OpenGL3:	 22
			Fmod:		 33
			ECS:         44
			Physics:     55

		ELSE:
			Runs 1st Test
		*/
		void Engine::RunTest(unsigned int TestNumber)
		{
			switch (TestNumber)
			{
			case 1:
			{	Test1 test1;
				Engine::RunGame(&test1);
				break; 
			}
			case 2:
			{	Test2 test2;
				Engine::RunGame(&test2);
				break;
			}
			case 3: 
			{
				Test3 test3;
				Engine::RunGame(&test3);
				break; 
			}
			case 4:
			{
				Test4 test4;
				Engine::RunGame(&test4);
				break; 
			}
			case 5:
			{
				Test5 test5;
				Engine::RunGame(&test5);
				break; 
			}
			case 11:
			{
				DirectX11Tests test11;
				Engine::RunGame(&test11);
				break;
			}
			case 22:
			{
				OpenGLTests test22;
				Engine::RunGame(&test22);
				break;
			}
			case 33:
			{
				FMODTests test33;
				Engine::RunGame(&test33);
				break; 
			}
			case 44:
			{
				ECSTests test44;
				Engine::RunGame(&test44);
				break; 
			}
			case 55:
			{
				PhysXTests test55;
				Engine::RunGame(&test55);
				break;
			}
			default:
			{	Test1 test1;
				Engine::RunGame(&test1);
			}
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
				GamePtr->ClockTime = clock.GetElapsedTime().AsSeconds();
				GamePtr->FrameTime = 1000.0f / ImGui::GetIO().Framerate;
				GamePtr->FPS = ImGui::GetIO().Framerate;

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

			Log.Info("[Engine] Game state changed to " + name + "\n");
		}

		void PrintIntroLog()
		{
			Log.Info("-------------------------- -Nuclear Engine- --------------------------\n");
			Log.Info("[Engine] Starting Engine...\n");
			Log.Info("[Engine] Version: ");
			Log.Info(MajorVersion);
			Log.Info(".");
			Log.Info(MinorVersion);
			Log.Info("\n[Engine] Built On: ");
			Log.Info(__DATE__);
			Log.Info("  At: ");
			Log.Info(__TIME__);
			Log.EndLine();

			Log.Info("[Engine] Built For: ");

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_32BIT
			Log.Info("Windows-PC 32 Bit\n");
#endif

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_64BIT
			Log.Info("Windows-PC 64 bit\n");
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4067)
#endif
			Log.Info("[Engine] Built With Renderer: ");


#ifdef 	NE_USE_CORE_OPENGL
			Log.Info("OpenGL 3.3 Core\n");
#endif

#ifdef 	NE_USE_DIRECTX11
			Log.Info("DirectX 11\n");
#endif

#ifdef 	NE_USE_RUNTIME_RENDER_API
			Log.Info("OpenGL 3.3 Core & DirectX 11\n");
#endif

#ifndef FMOD_NOT_INCLUDED
			Log.Info("[Engine] Built With FMOD AudioEngine\n");
#else
			Log.Warning("[Engine] Engine built without FMOD AudioEngine\n");
#endif

		}

	}
}