#include "Common.h"
#include "SampleSelector.h"
#include <iostream>

Diligent::RENDER_DEVICE_TYPE SelectRenderer()
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
		return Diligent::RENDER_DEVICE_TYPE_D3D11;
	case 2:
		return Diligent::RENDER_DEVICE_TYPE_D3D12;
	case 3:
		return Diligent::RENDER_DEVICE_TYPE_GL;
	case 4:
		return Diligent::RENDER_DEVICE_TYPE_VULKAN;
	}

	return Diligent::RENDER_DEVICE_TYPE_D3D11;
}

int main(int argc, char* argv[])
{
	Core::EngineStartupDesc desc;

	/*std::cout << "Auto Initialize? \n"
		<< "1) Yes \n"
		<< "2) No \n";
	*/
//	int i;
//	std::cin >> i;
//	if (i == 2)
	//{
	//	desc.Renderer = SelectRenderer();
	//	Core::Engine::GetInstance().Start(desc);

	//}
	//else
	//{
	//	EngineStartupDesc desc;
	//	Core::Engine::GetInstance().Start(desc);
	//}

	//EngineStartupDesc desc;


	desc.mEngineWindowDesc.WindowWidth = 1280;
	desc.mEngineWindowDesc.WindowHeight = 720;
	desc.mScriptingClientDllName = "SamplesScripts.dll";
	desc.mScriptingAssemblyNamespace = "Samples";
	Core::Engine::GetInstance().Start(desc);
	Core::Path::mReservedPaths["@CommonAssets@"] = "../Assets/Common";
	Assets::AssetLibrary::GetInstance().mPath = "../Assets/Imported";

	while (!Core::Engine::GetInstance().ShouldClose())
	{
		SampleSelector selector;
		Core::Engine::GetInstance().LoadClient(&selector);
	}

	return 0;
}