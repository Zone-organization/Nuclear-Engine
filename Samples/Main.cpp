#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"
#include "Playground.h"
#include "Sandbox.h"
#include <iostream>

void Start()
{
	char choice;
	std::cout << "--Choose A Renderer: \nA) OpenGL 3 \nB) DirectX 11\n";
	std::cin >> choice;
	Core::EngineStartupDesc desc;
	if (choice == 'a') {
		desc.mAppdesc.Renderer =	 Core::RenderAPI::OpenGL3;
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
	std::cout
		<< "-----[Select the sample you want]-----\n"

		<< "---Engine Samples:\n"
		<< "1) Sample 1 - Lighted Scene - introduction to Camera & Input & ECS & RenderSystem\n"
		<< "2) Sample 2 - Sponza Scene - introduction to Advanced RenderSystem Graphical Techniques.\n"
		<< "3) Sample 3 - 2D Scene - introduction to 2D Sprites & SpriteRenderSystem & GUI.\n"
		<< "----------------[Other]---------------\n"
		<< "x) Playground - testing new stuff (Currently Normal Mapping)\n"
		<< "y) Sandbox - testing new stuff (Currently Bloom)\n"
		<< "z) Tests\n";
	

	char choice;
	std::cin >> choice;
	if (choice == '1') {
		Start();
		Sample1 demo;
		Core::Engine::RunGame(&demo);
	}
	else if (choice == '2') {
		Start();
		Sample2 demo;
		Core::Engine::RunGame(&demo);
	}
	else if (choice == '3') {
		Start();
		Sample3 demo;
		Core::Engine::RunGame(&demo);
	}
	else if (choice == 'x') {
		Start();
		Playground game;
		Core::Engine::RunGame(&game);
	}
	else if (choice == 'y') {
		Start();
		Sandbox game;
		Core::Engine::RunGame(&game);
	}	
	else if (choice == 'z')
	{
		Start();
		std::cout << "Enter The test number: ";
		Uint32 testno;
		std::cin >> testno;
		Core::Engine::RunTest(testno);
	}
	else if (choice == 'H')
	{
		/*std::string TexUp("Texture.png");
		std::string TexDown("texture.png");
		std::string TexPathed("Assets/Common/Textures/Texture.png");

		Platform::Clock clock;
		clock.Restart();
		auto I0 = Utilities::Hash(TexUp);
		std::cout << clock.GetElapsedTime().AsMicroseconds() << "\n";
		clock.Restart();
		auto I1 = Utilities::Hash(TexDown);
		std::cout << clock.GetElapsedTime().AsMicroseconds() << "\n";
		clock.Restart();

		auto I2 = Utilities::Hash(TexPathed);
		std::cout << clock.GetElapsedTime().AsMicroseconds() << "\n";
		clock.Restart();
		*/
	}
	Core::Engine::Shutdown();

	return 0;
}