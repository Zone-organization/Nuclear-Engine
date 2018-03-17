#define NUCLEAR_ENGINE_MAIN_IMPLEMENTATION
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

	Core::ApplicationDesc desc;
	if (choice == 'a') {
		desc.renderer =	 Core::RenderAPI::OpenGL3;
	}
	else if (choice == 'b')
	{
		desc.renderer = Core::RenderAPI::DirectX11;
	}
	else {
		std::cout << "Wrong choice thus setting to DirectX 11 as default on window.\n";
		desc.renderer = Core::RenderAPI::DirectX11;
	}
	Core::Engine::Initialize(desc);
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
		Core::Engine::Run(&demo);
	}
	else if (choice == '2') {
		Start();
		Sample2 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == '3') {
		Start();
		Sample3 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == 'x') {
		Start();
		Playground game;
		Core::Engine::Run(&game);
	}
	else if (choice == 'y') {
		Start();
		Sandbox game;
		Core::Engine::Run(&game);
	}	
	else if (choice == 'z')
	{
		Start();
		std::cout << "Enter The test number: ";
		uint testno;
		std::cin >> testno;
		Core::Engine::Run(testno);
	}

	Core::Engine::ShutDown();

	return 0;
}