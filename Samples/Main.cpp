#include "Common.h"
#include "Sample1.h"
#include <iostream>

void Start()
{
	char choice;
	std::cout << "--Choose A Renderer: \nA) OpenGL 4.5 \nB) DirectX 11\n";
	std::cin >> choice;
	Core::EngineStartupDesc desc;
	if (choice == 'a') {
		desc.mAppdesc.Renderer = Core::RenderAPI::OpenGL4_5;
	}
	else if (choice == 'b')
	{
		desc.mAppdesc.Renderer = Core::RenderAPI::DirectX11;
	}
	else {
		std::cout << "Wrong choice thus setting to DirectX 11 as default on window.\n";
		desc.mAppdesc.Renderer = Core::RenderAPI::DirectX11;
	}
	Core::Engine::Start(desc);
}

int main(int argc, char* argv[])
{
	Start();
	Sample1 demo;
	Core::Engine::RunGame(&demo);

	Core::Engine::Shutdown();

	return 0;
}