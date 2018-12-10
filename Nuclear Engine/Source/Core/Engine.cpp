#include <Core\Engine.h>
#include <Base\NE_Common.h>
#include <Base\Utilities\Timer.h>
#include <Core\Input.h>
#include <Core\Application.h>
#include <Engine\Audio\AudioEngine.h>
#include <FMOD\includer.h> 
//#include <Engine\Graphics\ImGui_Binding.h>
//#include <Engine\Graphics\ImGui_Renderer.h>

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
			//Graphics::ImGui_Binding::Initialize(Application::MainWindow);
			//Graphics::ImGui_Renderer::Initialize();

			//if(desc.InitAudioEngine)
			//	Audio::AudioEngine::Initialize();

			GamePtr = &Defaultgame;
			return true;
		}

		void Engine::Shutdown()
		{
			Log.Info("[Engine] Shutting Down Engine.");

			GamePtr = &Defaultgame;
			Core::Application::Shutdown();
			Audio::AudioEngine::Shutdown();
			//Graphics::ImGui_Renderer::Shutdown();
		}

		void Engine::RunGame(Game * _YourGame)
		{
			GamePtr = _YourGame;
			Log.Info("[Engine] Running Game.");

			if (GamePtr->GetGameInfo() != nullptr)
			{
				Log.Info("[Engine] Game Name: " + std::string(GamePtr->GetGameInfo()->Name));
				Log.Info("[Engine] Game Version: " + std::string(GamePtr->GetGameInfo()->Version));
				Log.Info("[Engine] Game Developer: " + std::string(GamePtr->GetGameInfo()->Developer));
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
			while (!Core::Application::PollEvents())
			{
				// per-frame time logic (ensure speed is constant through all platforms)
				float currentFrame = static_cast<float>(timer.GetElapsedTimeInSeconds());
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				GamePtr->ClockTime = static_cast<float>(timer.GetElapsedTimeInSeconds());
				//Graphics::ImGui_Binding::Update(*Core::Application::MainWindow, deltaTime);

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

			Log.Info("[Engine] Game state changed to " + name);
		}

		void PrintIntroLog()
		{
			Log.Initialize();
			Log.Info("[-Nuclear Engine-]");
			Log.Info("[Engine] Starting Engine...");
			Log.Info("[Engine] Engine Build: " + MajorVersion + '.' + MinorVersion + "  On: " + __DATE__ + "  At: " + __TIME__);

			std::string EngineOS("[Engine] Built For: ");

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_32BIT
			EngineOS + "Windows-PC 32 Bit";
#endif

#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_64BIT
			EngineOS + "Windows-PC 64 bit";
#endif

#ifdef 	_DEBUG
			EngineOS + "  [DEBUG Build]";
#endif

#ifdef 	NDEBUG
			EngineOS + "  [RELEASE Build]";
#endif
			Log.Info(EngineOS);

#ifdef _MSC_VER
#pragma warning(disable : 4067)
#endif

#ifndef FMOD_NOT_INCLUDED
			Log.Info("[Engine] Built With FMOD AudioEngine");
#else
			Log.Warning("[Engine] Engine built without FMOD AudioEngine.");
#endif
#ifndef PHYSX_NOT_INCLUDED
			Log.Info("[Engine] Built With NVIDIA PhysX");
#else
			Log.Warning("[Engine] Engine built without NVIDIA PhysX.");
#endif
		}

	}
}