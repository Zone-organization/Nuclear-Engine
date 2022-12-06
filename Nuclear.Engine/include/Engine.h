#pragma once
#include <Client.h>
#include <Core\Window.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>

namespace Nuclear
{
	struct EngineStartupDesc
	{
		Core::WindowDesc mEngineWindowDesc;
		Uint32 Samples = 2;

		bool DebugRenderAPI = false;
		bool VSync = true;
		bool AutoSelectRenderer = true;

		bool AutoInitGraphicsEngine = true;
		bool AutoInitAudioEngine = true;
		bool AutoInitPhysXEngine = true;
		bool AutoInitScriptingEngine = true;
		std::string mScriptingClientDllName = "ClientScripts.dll";
		std::string mScriptingAssemblyNamespace = "ClientScripts";
		//Enables some debugging options that slows down performance in runtime
		bool Debug = DEBUG_TRUE_BOOL;
		bool EnableLogging = DEBUG_TRUE_BOOL;
	};

	class NEAPI Engine
	{
	public:
		Engine(Engine const&) = delete;
		void operator=(Engine const&) = delete;

		static Engine& GetInstance();

		enum class State { Initializing, Loading, Rendering, ExitingRendering, Shuttingdown };

		bool Start(const EngineStartupDesc& desc);

		void Shutdown();

		void BeginFrame();

		void EndFrame();

		Core::Window* GetMainWindow();
	
		void LoadClient(Client* client);
		void EndClient();

		void SetState(const State& state);

		bool ShouldClose();

		bool isDebug();

		Client* GetClient();

	private:
		Engine();
		Client* pClient;
		Core::Window MainWindow;

		Engine::State Engine_State;
		bool gisDebug = DEBUG_TRUE_BOOL;
		void MainLoop();
	};
}