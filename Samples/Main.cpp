#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"
#include <iostream>

//Core::RenderAPI SelectRenderer()
//{
//	std::cout << "Select Renderer: \n"
//		<< "1) DirectX 11 \n"
//		<< "2) DirectX 12 \n"
//		<< "3) OpenGL 4 \n"
//		<< "4) Vulkan \n";
//
//	int i;
//	std::cin >> i;
//
//	switch (i)
//	{
//	case 1:
//		return Core::RenderAPI::DirectX11;
//	case 2:
//		return Core::RenderAPI::DirectX12;
//	case 3:
//		return Core::RenderAPI::OpenGL;
//	case 4:
//		return Core::RenderAPI::Vulkan;
//	}
//
//	return Core::RenderAPI::DirectX12;
//}

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

	Core::Path::mReservedPaths["@CommonAssets@"] = "Assets/Common";
	Core::Path::mReservedPaths["@NuclearAssets@"] = "Assets/NuclearEngine";

		Core::EngineStartupDesc desc;
		Core::Engine::Start(desc);

	//}

	//	std::cout << "Select Sample:\n"
	//		<< "1) Sample1 : BlinnPhong rendering of some models \n"
	//		<< "2) Sample2 : PBR Rendering \n"
	//		<< "3) Sample3 : PhysX Integration\n";

	//std::cin >> i;
	//if (i == 1)
	//{
	//	Sample1 demo;
	//	Core::Engine::RunGame(&demo);
	//}
	//else if (i == 2)
	//{
		Sample2 demo;
		Core::Engine::RunGame(&demo);
	//}
	//else {
	//	Sample3 demo;
	//	Core::Engine::RunGame(&demo);
	//}


	Core::Engine::Shutdown();

	return 0;
}