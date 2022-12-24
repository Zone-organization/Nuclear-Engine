#include <Core/Engine.h>
#include <Utilities\Timer.h>
#include <Platform\Window.h>
#include <Platform\Input.h>

#include <Utilities\Logger.h>

#include <Graphics\Context.h>
#include <Assets\DefaultMeshes.h>

#include "..\Graphics\ImGUI\imgui_impl_sdl.h"
#include "..\Graphics\ImGUI\imgui_impl.h"

#include <SDL\include\SDL.h>

#include <filesystem>

//Sub-engines
#include <Audio\AudioEngine.h>
#include <Graphics\GraphicsEngine.h>
#include <PhysX\PhysXEngine.h>
#include <Scripting/ScriptingEngine.h>
#include <Rendering\RenderingEngine.h>

#include <Assets/AssetManager.h>

//Dependencies Linking
#pragma comment(lib,"assimp-vc143-mt.lib")

#pragma comment(lib,"freetype.lib")
#pragma comment(lib,"msdf-atlas-gen.lib")
#pragma comment(lib,"msdfgen.lib")

//Diligent Linking
#pragma comment(lib,"Diligent-Common.lib")

#ifdef _DEBUG
#pragma comment(lib,"GraphicsEngineD3D11_64d.lib")
#pragma comment(lib,"GraphicsEngineD3D12_64d.lib")
#pragma comment(lib,"GraphicsEngineOpenGL_64d.lib")
#pragma comment(lib,"GraphicsEngineVK_64d.lib")
#else
#pragma comment(lib,"GraphicsEngineD3D11_64r.lib")
#pragma comment(lib,"GraphicsEngineD3D12_64r.lib")
#pragma comment(lib,"GraphicsEngineOpenGL_64r.lib")
#pragma comment(lib,"GraphicsEngineVK_64r.lib")
#endif

#pragma comment(lib,"Diligent-GraphicsAccessories.lib")
#pragma comment(lib,"Diligent-GraphicsTools.lib")
#pragma comment(lib,"Diligent-BasicPlatform.lib")
#pragma comment(lib,"Diligent-Win32Platform.lib")


#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2Main.lib")


namespace Nuclear {
	namespace Core {

		static std::string MajorVersion = "0";
		static std::string MinorVersion = "001";

		void PrintIntroLog();

		void ResizeCallback(int Width, int Height)
		{
			if (Width == 0 && Height == 0)
			{
				return;
			}
			Engine::GetInstance().GetClient()->OnWindowResize(Width, Height);
		}
		bool Engine::Start(const EngineStartupDesc& desc)
		{
			PrintIntroLog();

			Core::Path::mReservedPaths["@NuclearAssets@"] = "../Assets/NuclearEngine";
			Core::Path::mReservedPaths["@CurrentPath@"] = std::filesystem::current_path().string();

			//Initialize SDL
			if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
			{
				NUCLEAR_FATAL("[Engine] SDL could not initialize! SDL_Error: {0}", SDL_GetError());
				return false;
			}

			if (!MainWindow.Create(desc.mEngineWindowDesc))
			{
				NUCLEAR_FATAL("[Engine] Failed To Create Window...");
			}

			Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Normal);

			if (desc.AutoInitGraphicsEngine)
			{
				Graphics::GraphicsEngineDesc GraphicsEngineDesc;
				GraphicsEngineDesc.pWindowHandle = GetMainWindow()->GetSDLWindowPtr();
				if (!Graphics::GraphicsEngine::GetInstance().Initialize(GraphicsEngineDesc))
				{
					NUCLEAR_FATAL("[Engine] Failed to initalize GraphicsEngine...");
					return false;
				}
			}

			if (desc.AutoInitAudioEngine)
			{
				Audio::AudioEngineDesc desc;

				if (!Audio::AudioEngine::GetInstance().Initialize(desc))
				{
					NUCLEAR_FATAL("[Engine] Failed to initalize AudioEngine...");
					return false;
				}
			}
			if (desc.AutoInitScriptingEngine)
			{
				namespace fs = std::filesystem;

				Scripting::ScriptingEngineDesc scdesc;
				fs::path monopath = std::filesystem::current_path().string() + "/mono";

				if (!fs::exists(monopath))
				{
					monopath = std::filesystem::current_path().string() + "/../mono";
					if (!fs::exists(monopath))
					{
						NUCLEAR_FATAL("[Engine] Failed to find mono runtime assemblies directory!");
					}
				}
				scdesc.mMonoRuntimeDir = monopath.string();
				scdesc.mScriptingCoreAssemblyDir = std::filesystem::current_path().string();
				scdesc.mClientAssemblyPath = std::filesystem::current_path().string() + "/" + desc.mScriptingClientDllName;
				scdesc.mClientNamespace = desc.mScriptingAssemblyNamespace;
				if (!Scripting::ScriptingEngine::GetInstance().Initialize(scdesc))
				{
					NUCLEAR_FATAL("[Engine] Failed to initalize ScriptingEngine...");
					return false;
				}
			}

			if (desc.AutoInitPhysXEngine)
			{
				PhysX::PhysXEngineDesc pxdesc;

				if (!PhysX::PhysXEngine::GetInstance().Initialize(pxdesc))
				{
					NUCLEAR_FATAL("[Engine] Failed to initalize PhysXEngine...");
					return false;
				}
			}

			if (desc.AutoInitRenderingEngine)
			{
				Rendering::RenderingEngineDesc redesc;
				redesc.RTWidth = desc.mEngineWindowDesc.WindowWidth;
				redesc.RTHeight = desc.mEngineWindowDesc.WindowHeight;;
									
				if (!Rendering::RenderingEngine::GetInstance().Initialize(redesc))
				{
					NUCLEAR_FATAL("[Engine] Failed to initalize RenderingEngine...");
					return false;
				}			
			}


			gisDebug = desc.Debug;

			Assets::AssetManager::GetInstance().Initialize();

			NUCLEAR_INFO("[Engine] Nuclear Engine has been initalized successfully!");
			return true;
		}

		inline Engine& Engine::GetInstance()
		{
			static Engine engine;

			return engine;
		}

		void Engine::Shutdown()
		{
			NUCLEAR_INFO("[Engine] Shutting Down Engine.");

			pClient = nullptr;
			Audio::AudioEngine::GetInstance().Shutdown();
			PhysX::PhysXEngine::GetInstance().Shutdown();
			Graphics::GraphicsEngine::GetInstance().Shutdown();
			MainWindow.Destroy();
			//Graphics::ImGui_Renderer::Shutdown();
			SDL_Quit();
			NUCLEAR_INFO("------------------- -Nuclear Engine Has Shutdown- -----------------------");
		}

		void Engine::BeginFrame()
		{
			ImGui_Impl_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}

		void Engine::EndFrame()
		{
			ImGui::Render();
			ImGui_Impl_RenderDrawData(ImGui::GetDrawData());
			Graphics::Context::GetInstance().GetSwapChain()->Present();
		}
		Platform::Window* Engine::GetMainWindow()
		{
			return &MainWindow;
		}

		void Engine::LoadClient(Client* client)
		{
			pClient = client;

			if (pClient != nullptr)
			{
				NUCLEAR_INFO("[Engine] Loading Client: '{0}' - Ver: '{1}' - Dev: '{2}'",
					pClient->GetClientInfo().mName,
					pClient->GetClientInfo().mVersion,
					pClient->GetClientInfo().mDeveloper);

				SetState(Engine::State::Initializing);
				pClient->Initialize();
				SetState(Engine::State::Loading);
				pClient->Load();

				Engine::MainLoop();
			}
		}

		void Engine::EndClient()
		{
			if (pClient != nullptr)
			{
				SetState(Engine::State::ExitingRendering);
				pClient->ExitRendering();
				SetState(Engine::State::Shuttingdown);
				pClient->Shutdown();
			}
			pClient = nullptr;
		}

		bool Engine::ShouldClose()
		{
			return mShouldClose;
		}

		bool Engine::isDebug()
		{
			return gisDebug;
		}

		Client* Engine::GetClient()
		{
			return pClient;
		}

		Engine::Engine()
		{

		}

		void Engine::MainLoop()
		{
			SetState(Engine::State::Rendering);

			Utilities::Timer timer;

			mShouldClose = false;

			//Event handler
			SDL_Event e;
			//Main Client Loop
			while (!mShouldClose && pClient != nullptr)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					switch (e.type)
					{
					case SDL_QUIT:
						mShouldClose = true;
						break;
					case SDL_WINDOWEVENT:
						if (e.window.event == SDL_WINDOWEVENT_RESIZED)
						{
							ResizeCallback(e.window.data1, e.window.data2);
						}
						break;
					case SDL_MOUSEMOTION:
						int w, h;
						MainWindow.GetSize(w,h);
						static int xpos = w / 2; 
						static int ypos = h / 2;
						xpos += e.motion.xrel;
						ypos += e.motion.yrel;
						pClient->OnMouseMovement(xpos, ypos);

						break;

					};
					ImGui_ImplSDL2_ProcessEvent(&e);
				}
				Platform::Input::GetInstance().Update();

				// per-frame time logic (ensure speed is constant through all platforms)
				float currentFrame = static_cast<float>(timer.GetElapsedTimeInSeconds());
				pClient->DeltaTime = currentFrame - pClient->LastFrame;
				pClient->LastFrame = currentFrame;
				pClient->ClockTime = static_cast<float>(timer.GetElapsedTimeInSeconds());

				BeginFrame();

				pClient->Update(pClient->DeltaTime);
				pClient->Render(pClient->DeltaTime);

				EndFrame();

			}
		}

		void Engine::SetState(const State& state)
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

			NUCLEAR_INFO("[Engine] Client state changed to '{0}'", name);
		}

		void PrintIntroLog()
		{
			NUCLEAR_INFO("-------------------------- -Nuclear Engine- --------------------------");
			NUCLEAR_INFO("------------------------- Zone Organization --------------------------");
			NUCLEAR_INFO("--------------------------------v7medz--------------------------------");
			NUCLEAR_INFO("[Engine] Starting Engine...");
			NUCLEAR_INFO("[Engine] Engine Build: '{0}'.'{1}'  On: '{2}' At: '{3}'", MajorVersion, MinorVersion, __DATE__, __TIME__);

			//			NUCLEAR_INFO("[Engine] Built For: ");

			//#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_32BIT
			//			NUCLEAR_INFO("Windows-PC 32 Bit");
			//#endif
			//
			//#ifdef 	NUCLEAR_PLATFORM_WINDOWS_PC_64BIT
			//			NUCLEAR_INFO("Windows-PC 64 bit");
			//#endif
			//
			//#ifdef 	_DEBUG
			//			NUCLEAR_INFO("  [DEBUG Build]\n");
			//#endif
			//
			//#ifdef 	NDEBUG
			//			NUCLEAR_INFO("  [RELEASE Build]\n");
			//#endif

#ifdef _MSC_VER
#pragma warning(disable : 4067)
#endif
		}
	}
}