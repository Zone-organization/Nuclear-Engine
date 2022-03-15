#include "Common.h"
#include "SampleSelector.h"
#include <iostream>
#include "Engine\Scripting\ScriptingEngine.h"

Core::RenderAPI SelectRenderer()
{
	std::cout << "Select Renderer: \n"
		<< "1) DirectX 11 \n"
		<< "2) DirectX 12 \n"
		<< "3) OpenGL 4 \n"
		<< "4) Vulkan \n";

	int i;
	std::cin >> i;

	switch (i)
	{
	case 1:
		return Core::RenderAPI::DirectX11;
	case 2:
		return Core::RenderAPI::DirectX12;
	case 3:
		return Core::RenderAPI::OpenGL;
	case 4:
		return Core::RenderAPI::Vulkan;
	}

	return Core::RenderAPI::DirectX12;
}

int main(int argc, char* argv[])
{

	//std::cout << "Auto Initialize? \n"
	//	<< "1) Yes \n"
	//	<< "2) No \n";

	//int i;
	//std::cin >> i;
	//if (i == 2)
	//{
	//	Core::EngineStartupDesc desc;
	//	desc.mAppdesc.Renderer = SelectRenderer();
	//	Core::Engine::Start(desc);

	//}
	//else
	//{
		Core::EngineStartupDesc desc;
		Core::Engine::Start(desc);
	//}
	Core::Path::mReservedPaths["@CommonAssets@"] = "Assets/Common";
	Core::Path::mReservedPaths["@NuclearAssets@"] = "Assets/NuclearEngine";

	bool haha = NuclearEngine::Scripting::ScriptingEngine::Initialize();

	SampleSelector selector;
	Core::Engine::SetGame(&selector);
	Core::Engine::LoadGame();
	Core::Engine::RunGame();
	Core::Engine::EndGame();
	Core::Engine::Shutdown();

	return 0;
}