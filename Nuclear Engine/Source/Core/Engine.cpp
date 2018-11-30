#include <Core\Engine.h>
#include <Base\NE_Common.h>
#include <Base\Utilities\Timer.h>
#include <Core\Input.h>
#include <Core\Application.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Audio\AudioEngine.h>
#include <FMOD\includer.h> 
#include <Engine\Graphics\ImGui_Binding.h>
#include <Engine\Graphics\ImGui_Renderer.h>

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
			Application::Start(desc.mAppdesc);
			Application::SetMouseInputMode(MouseInputMode::Normal);

			//init sub-engines
			Graphics::GraphicsEngine::Initialize();
			Graphics::ImGui_Binding::Initialize(Application::MainWindow);
			Graphics::ImGui_Renderer::Initialize();

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
			Graphics::ImGui_Renderer::Shutdown();
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
		
		bool Engine::ShouldClose()
		{
			return Core::Application::ShouldClose;
		}

		Game * Engine::GetGame()
		{
			return GamePtr;
		}
	
		void Engine::Game_Loop_Render()
		{
			SetState(Engine::State::Rendering);

			Base::Utilities::Timer timer;
			
			//Main Game Loop
			while (!Core::Application::ShouldClose)
			{
				// per-frame time logic (ensure speed is constant through all platforms)
				float currentFrame = static_cast<float>(timer.GetElapsedTimeInSeconds());
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				GamePtr->ClockTime = static_cast<float>(timer.GetElapsedTimeInSeconds());
				Graphics::ImGui_Binding::Update(*Core::Application::MainWindow, deltaTime);

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
			Log.Info("[Engine] Engine Build: ");
			Log.Info(MajorVersion);
			Log.Info(".");
			Log.Info(MinorVersion);
			Log.Info("  On: ");
			Log.Info(__DATE__);
			Log.Info("  At: ");
			Log.Info(__TIME__);
			Log.EndLine();

			Log.Info("[Engine] Built For: ");

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_32BIT
			Log.Info("Windows-PC 32 Bit");
#endif

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_64BIT
			Log.Info("Windows-PC 64 bit");
#endif

#ifdef 	_DEBUG
			Log.Info("  [DEBUG Build]\n");
#endif

#ifdef 	NDEBUG
			Log.Info("  [RELEASE Build]\n");
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4067)
#endif
			Log.Info("[Engine] Built-In Renderer");


#ifdef 	NE_USE_CORE_OPENGL
			Log.Info(": OpenGL 4.5 Core\n");
#endif

#ifdef 	NE_USE_DIRECTX11
			Log.Info(": DirectX 11\n");
#endif

#ifdef 	NE_USE_RUNTIME_RENDER_API
			Log.Info("s: OpenGL 4.5 Core & DirectX 11\n");
#endif

#ifndef FMOD_NOT_INCLUDED
			Log.Info("[Engine] Built With FMOD AudioEngine\n");
#else
			Log.Warning("[Engine] Engine built without FMOD AudioEngine.\n");
#endif
#ifndef PHYSX_NOT_INCLUDED
			Log.Info("[Engine] Built With NVIDIA PhysX\n");
#else
			Log.Warning("[Engine] Engine built without NVIDIA PhysX.\n");
#endif
		}

	}
}