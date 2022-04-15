#include <Engine.h>
#include <Core\Utilities\Timer.h>
#include <Core\Window.h>
#include <Core\Logger.h>

#include <Engine\Audio\AudioEngine.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Assets\DefaultMeshes.h>

#include "Engine\Graphics\ImGUI\imgui_impl_glfw.h"
#include "Engine\Graphics\ImGUI\imgui_impl.h"
#include <GLFW/include/GLFW/glfw3.h>

//Dependencies Linking
#pragma comment(lib,"Nuclear.Core.lib")

#pragma comment(lib,"assimp-vc143-mtd.lib")

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


namespace Nuclear {
	namespace Core {

		static std::string MajorVersion = "0";
		static std::string MinorVersion = "001";

		static Engine* engine = nullptr;

		void PrintIntroLog();
		void ResizeCallback(GLFWwindow* window, int Width, int Height)
		{
			Core::Engine::GetInstance()->GetMainWindow()->UpdateSize();
			Core::Engine::GetInstance()->GetGame()->OnWindowResize(Width, Height);
		}
		bool Engine::Start(const EngineStartupDesc& desc)
		{
			PrintIntroLog();

			//Create platform specific app (window)
			Core::Window::InitializeGLFW();

			if (!MainWindow.Create(desc.mEngineWindowDesc))
			{
				Log.FatalError("[Engine] Failed To Create Window...\n");
			}
			//Application::Start(desc.mAppdesc);

			glfwSetFramebufferSizeCallback(MainWindow.GetRawWindowPtr(), ResizeCallback);

			MainWindow.SetMouseInputMode(Core::Input::MouseInputMode::Normal);

			if (!Graphics::GraphicsEngine::Initialize(desc.mGraphicsEngineDesc))
			{
				Log.FatalError("[Engine] GraphicsEngine Failed to initalize...\n");
				return false;
			}
			//Initialize Context
			Graphics::Context::Initialize(desc.Renderer, desc.mGraphicsEngineDesc);
			Assets::DefaultMeshes::Initialize();


			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForOther(MainWindow.GetRawWindowPtr(), true);
			ImGui_Impl_Init();
			ImGui_Impl_CreateDeviceObjects();
			Log.Info("[Engine] ImGUI Initalized.\n");


			if (desc.AutoInitAudioEngine)
				Audio::AudioEngine::Initialize();



			if (desc.AutoInitPhysXEngine)
			{
				if (!PhysX::PhysXEngine::Initialize(desc.mPhysXEngineDesc))
				{
					Log.FatalError("[Engine] PhysXEngine Failed to auto Initialize!\n");
					return false;
				}
			}
			gisDebug = desc.Debug;
			GamePtr = &Defaultgame;

			int width2, height2;
			GetMainWindow()->GetSize(width2, height2);

			Log.Info("[Engine] Nuclear Engine has been initalized successfully!\n");
			return true;
		}

		Engine* Engine::GetInstance()
		{
			if (engine == nullptr)
			{
				engine = new Engine();
			}
			return engine;

		}

		void Engine::Shutdown()
		{
			Log.Info("[Engine] Shutting Down Engine.\n");

			GamePtr = &Defaultgame;
			Graphics::Context::ShutDown();
			Audio::AudioEngine::Shutdown();
			Graphics::GraphicsEngine::Shutdown();
			MainWindow.Destroy();
			//Graphics::ImGui_Renderer::Shutdown();

			Log.Info("------------------------ -Engine Has Shutdown- -----------------------\n");
			Log.Info("-------------------------- -Nuclear Engine- --------------------------\n");
		}

		void Engine::BeginFrame()
		{
			ImGui_Impl_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void Engine::EndFrame()
		{
			ImGui::Render();
			ImGui_Impl_RenderDrawData(ImGui::GetDrawData());
			Graphics::Context::GetSwapChain()->Present();
		}
		Window* Engine::GetMainWindow()
		{
			return &MainWindow;
		}
		void Engine::SetGame(Game* YourGame)
		{
			GamePtr = YourGame;
		}
		void Engine::LoadGame()
		{
			if (GamePtr != nullptr)
			{
				if (GamePtr->GetGameInfo() != nullptr)
				{
					Log.Info(std::string(
						"[Engine] Loading Game: " + std::string(GamePtr->GetGameInfo()->Name)
						+ " - Ver: " + std::string(GamePtr->GetGameInfo()->Version)
						+ " - Dev: " + std::string(GamePtr->GetGameInfo()->Developer) + "\n"));
				}
				else {
					Log.Info("[Engine] Loading Unnamed Game.\n");

				}
				SetState(Engine::State::Initializing);
				GamePtr->Initialize();
				SetState(Engine::State::Loading);
				GamePtr->Load();
			}
		}

		void Engine::RunGame()
		{
			if (GamePtr != nullptr)
			{
				Log.Info("[Engine] Running Game.\n");

				Engine::Game_Loop_Render();
			}
		}

		void Engine::EndGame()
		{
			if (GamePtr != nullptr)
			{
				SetState(Engine::State::ExitingRendering);
				GamePtr->ExitRendering();
				SetState(Engine::State::Shuttingdown);
				GamePtr->Shutdown();
			}
			GamePtr = nullptr;
		}

		bool Engine::ShouldClose()
		{
			return MainWindow.ShouldClose();
		}

		bool Engine::isDebug()
		{
			return gisDebug;
		}

		Game* Engine::GetGame()
		{
			return GamePtr;
		}

		void Engine::Game_Loop_Render()
		{
			SetState(Engine::State::Rendering);

			Core::Utilities::Timer timer;

			double SavedX = 0, SavedY = 0;

			//Main Game Loop
			while (!MainWindow.ShouldClose() && GamePtr != nullptr)
			{
				MainWindow.PollEvents();

				MainWindow.UpdateSize();

				// per-frame time logic (ensure speed is constant through all platforms)
				float currentFrame = static_cast<float>(timer.GetElapsedTimeInSeconds());
				GamePtr->DeltaTime = currentFrame - GamePtr->LastFrame;
				GamePtr->LastFrame = currentFrame;
				GamePtr->ClockTime = static_cast<float>(timer.GetElapsedTimeInSeconds());

				BeginFrame();

				//Mouse Movement Callback
				double MousePosX, MousePosY;
				MainWindow.GetMousePosition(&MousePosX, &MousePosY);
				if (SavedX != MousePosX || SavedY != MousePosY)
				{
					SavedX = MousePosX;
					SavedY = MousePosY;
					GamePtr->OnMouseMovement(static_cast<int>(SavedX), static_cast<int>(SavedY));
				}

				GamePtr->Update(GamePtr->DeltaTime);
				GamePtr->Render(GamePtr->DeltaTime);

				EndFrame();
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
			Log.Info("------------------------- Zone Organization --------------------------\n");
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
		}
	}
}