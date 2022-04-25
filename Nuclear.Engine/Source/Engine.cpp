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

#pragma comment(lib,"assimp-vc143-mt.lib")

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
			Core::Logger::Initialize();
			PrintIntroLog();

			//Create platform specific app (window)
			Core::Window::InitializeGLFW();

			if (!MainWindow.Create(desc.mEngineWindowDesc))
			{
				NUCLEAR_FATAL("[Engine] Failed To Create Window...");
			}
			//Application::Start(desc.mAppdesc);

			glfwSetFramebufferSizeCallback(MainWindow.GetRawWindowPtr(), ResizeCallback);

			MainWindow.SetMouseInputMode(Core::Input::MouseInputMode::Normal);

			if (!Graphics::GraphicsEngine::Initialize(desc.mGraphicsEngineDesc))
			{
				NUCLEAR_FATAL("[Engine] GraphicsEngine Failed to initalize...");
				return false;
			}
			//Initialize Context
			Graphics::Context::Initialize(desc.Renderer, desc.mGraphicsEngineDesc);
			Assets::DefaultMeshes::Initialize();


			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForOther(MainWindow.GetRawWindowPtr(), true);
			ImGui_Impl_Init();
			ImGui_Impl_CreateDeviceObjects();
			NUCLEAR_INFO("[Engine] ImGUI Initalized.");


			if (desc.AutoInitAudioEngine)
				Audio::AudioEngine::Initialize();



			if (desc.AutoInitPhysXEngine)
			{
				if (!PhysX::PhysXEngine::Initialize(desc.mPhysXEngineDesc))
				{
					NUCLEAR_FATAL("[Engine] PhysXEngine Failed to auto Initialize!");
					return false;
				}
			}
			gisDebug = desc.Debug;
			GamePtr = &Defaultgame;

			int width2, height2;
			GetMainWindow()->GetSize(width2, height2);

			NUCLEAR_INFO("[Engine] Nuclear Engine has been initalized successfully!");
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
			NUCLEAR_INFO("[Engine] Shutting Down Engine.");

			GamePtr = &Defaultgame;
			Graphics::Context::ShutDown();
			Audio::AudioEngine::Shutdown();
			Graphics::GraphicsEngine::Shutdown();
			MainWindow.Destroy();
			//Graphics::ImGui_Renderer::Shutdown();

			NUCLEAR_INFO("------------------------ -Engine Has Shutdown- -----------------------");
			NUCLEAR_INFO("-------------------------- -Nuclear Engine- --------------------------");
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
					NUCLEAR_INFO("[Engine] Loading Game: '{0}' - Ver: '{1}' - Dev: '{2}'",
						GamePtr->GetGameInfo()->Name,
						GamePtr->GetGameInfo()->Version,
						GamePtr->GetGameInfo()->Developer);
				}
				else {
					NUCLEAR_INFO("[Engine] Loading Unnamed Game.");

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
				NUCLEAR_INFO("[Engine] Running Game.");

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

			NUCLEAR_INFO("[Engine] Game state changed to '{0}'" , name );
		}

		void PrintIntroLog()
		{
			NUCLEAR_INFO("-------------------------- -Nuclear Engine- --------------------------");
			NUCLEAR_INFO("------------------------- Zone Organization --------------------------");
			NUCLEAR_INFO("[Engine] Starting Engine...");
			NUCLEAR_INFO("[Engine] Engine Build: '{0}'.'{1}'  On: '{2}' At: '{3}'", MajorVersion , MinorVersion , __DATE__ , __TIME__);

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