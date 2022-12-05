#include <Engine.h>
#include <Core\Utilities\Timer.h>
#include <Core\Window.h>
#include <Core\Logger.h>

#include <Engine\Graphics\Context.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Importers\FreeImageImporter.h>

#include "Engine\Graphics\ImGUI\imgui_impl_glfw.h"
#include "Engine\Graphics\ImGUI\imgui_impl.h"

#include <GLFW/include/GLFW/glfw3.h>

#include <filesystem>

//Sub-engines
#include <Engine\Audio\AudioEngine.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\PhysX\PhysXEngine.h>
#include <Engine/Scripting/ScriptingEngine.h>

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


namespace Nuclear {
	static std::string MajorVersion = "0";
	static std::string MinorVersion = "001";

	void PrintIntroLog();

	void ResizeCallback(GLFWwindow* window, int Width, int Height)
	{
		if (Width == 0 && Height == 0)
		{
			return;
		}
		Engine::GetInstance().GetMainWindow()->UpdateSize();
		Engine::GetInstance().GetClient()->OnWindowResize(Width, Height);
	}
	bool Engine::Start(const EngineStartupDesc& desc)
	{
		PrintIntroLog();

		Core::Path::mReservedPaths["@NuclearAssets@"] = "../Assets/NuclearEngine";

		//Create platform specific app (window)
		Core::Window::InitializeGLFW();

		if (!MainWindow.Create(desc.mEngineWindowDesc))
		{
			NUCLEAR_FATAL("[Engine] Failed To Create Window...");
		}

		glfwSetFramebufferSizeCallback(MainWindow.GetRawWindowPtr(), ResizeCallback);

		MainWindow.SetMouseInputMode(Core::Input::MouseInputMode::Normal);

		if (desc.AutoInitGraphicsEngine)
		{
			Graphics::GraphicsEngineDesc GraphicsEngineDesc;
			GraphicsEngineDesc.pWindowHandle = GetMainWindow()->GetRawWindowPtr();
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

			if (!Scripting::ScriptingEngine::GetInstance().Initialize(scdesc))
			{
				NUCLEAR_FATAL("[Engine] Failed to initalize ScriptingEngine...");
				return false;
			}
		}

		if (desc.AutoInitPhysXEngine)
		{
			PhysX::PhysXEngineDesc desc;

			if (!PhysX::PhysXEngine::GetInstance().Initialize(desc))
			{
				NUCLEAR_FATAL("[Engine] Failed to initalize PhysXEngine...");
				return false;
			}
		}
		gisDebug = desc.Debug;

		Importers::FreeImage::Initialize();

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
		Importers::FreeImage::Shutdown();

		pClient = nullptr;
		Audio::AudioEngine::GetInstance().Shutdown();
		PhysX::PhysXEngine::GetInstance().Shutdown();
		Graphics::GraphicsEngine::GetInstance().Shutdown();
		MainWindow.Destroy();
		//Graphics::ImGui_Renderer::Shutdown();

		NUCLEAR_INFO("------------------- -Nuclear Engine Has Shutdown- -----------------------");
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
		Graphics::Context::GetInstance().GetSwapChain()->Present();
	}
	Core::Window* Engine::GetMainWindow()
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
		return MainWindow.ShouldClose();
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

		Core::Utilities::Timer timer;

		double SavedX = 0, SavedY = 0;

		//Main Client Loop
		while (!MainWindow.ShouldClose() && pClient != nullptr)
		{
			MainWindow.PollEvents();

			MainWindow.UpdateSize();

			// per-frame time logic (ensure speed is constant through all platforms)
			float currentFrame = static_cast<float>(timer.GetElapsedTimeInSeconds());
			pClient->DeltaTime = currentFrame - pClient->LastFrame;
			pClient->LastFrame = currentFrame;
			pClient->ClockTime = static_cast<float>(timer.GetElapsedTimeInSeconds());

			BeginFrame();

			//Mouse Movement Callback
			double MousePosX, MousePosY;
			MainWindow.GetMousePosition(&MousePosX, &MousePosY);
			if (SavedX != MousePosX || SavedY != MousePosY)
			{
				SavedX = MousePosX;
				SavedY = MousePosY;
				pClient->OnMouseMovement(static_cast<int>(SavedX), static_cast<int>(SavedY));
			}

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
