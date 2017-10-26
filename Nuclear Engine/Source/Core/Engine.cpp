#include <Core\Engine.h>
#include <NE_Common.h>
#include <NuclearPlatform\Application.h>
#include <NuclearPlatform\Input.h>
using namespace NuclearPlatform;
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

			static bool HasBeenInitialized;

			static std::string MajorVersion = "0";
			static std::string MinorVersion = "001";

			static Game * YourGame;
				
			bool Engine::Initialize(std::wstring WindowTitle, unsigned int width, unsigned int height, bool Debug, bool DisableLog )
			{

				if (HasBeenInitialized != true)
				{
					Application::Create(WindowTitle, width, height,false);
					Log->Info("-------------------------- -Nuclear Engine- --------------------------\n");
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

					HasBeenInitialized = true;
					return true;
				}
				else if (HasBeenInitialized == true) {
					Log->FatalError("Engine Has Been Already Initialized!\n");
					return false;
				}
				return true;
			}

			void Engine::ShutDown()
			{
				Log->Info("[Engine] Shutting Down Engine.\n");
				Application::Delete();
			}

			void Engine::Run(Game * _YourGame)
			{	
				YourGame = _YourGame;

				 Engine::Game_Initialize();
				 Application::Display();
				 Engine::Game_StartupLoad();
				 Engine::Game_Load();
				 Log->Info("[Engine] Starting Renderering!\n");
				 Engine::Game_Render();
				 Engine::Game_ExitRender();
				 Engine::Game_Shutdown();
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
				//Engine::State = GameState::Initializing;
				YourGame->Initialize();
			}

			void Engine::Game_StartupLoad()
			{
				//Engine::State = GameState::StartupLoad;
				YourGame->StartupLoad();
			}

			void Engine::Game_Load()
			{
				//Engine::State = GameState::Loading;
				YourGame->Load();
			}

			void Engine::Game_Render()
			{
				//Engine::State = GameState::Rendering;
				NuclearPlatform::Clock clock;
				//Main Game Loop
				while (Core::Engine::ShouldClose() != true)
				{
					YourGame->PreRender(clock.GetElapsedTime().AsSeconds());
					YourGame->Render();
					YourGame->PostRender();
				}
			}

			void Engine::Game_ExitRender()
			{
				//Engine::State = GameState::ExitingRendering;
				YourGame->ExitRendering();
			}

			void Engine::Game_Shutdown()
			{
				//Engine::State = GameState::Shuttingdown;
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