#include "Common.h"
#define PTE_IMPLEMENTATION
#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"

void Start()
{
	char choice;
	std::cout << "--Choose A Renderer: \nA) OpenGL 3 \nB) DirectX 11\n";
	std::cin >> choice;

	//Initialize Engine
	Core::Engine::Initialize(L"Nuclear Engine", _Width_, _Height_);

	if (choice == 'a') {
		Core::Context::Initialize(Core::RenderAPI::OpenGL3);
	}
	else if (choice == 'b')
	{
		Core::Context::Initialize(Core::RenderAPI::DirectX11);
	}
	else {
		std::cout << "Wrong Choice Setting to OpenGL As Default\n";
		Core::Context::Initialize(Core::RenderAPI::OpenGL3);
	}
}

int main(int argc, char* argv[])
{
	std::cout
		<< "-----[Select the demo you want]-----"
		<< "---Low level API demos:\n"
		<< "1) Demo 1 - Colored Triangle - introduction to Shaders & Vertex Buffers & Input Layouts\n"
		<< "2) Demo 2 - Textured Rectangle -  introduction to Index Buffers & Textures\n"
		<< "3) Demo 3 - Static Textured Cube - introduction to Transformations & Constant Buffers\n"

		<< "---High level engine demos:\n"
		<< "4) Demo 4 - Dynamic Textured Cube - introduction to Camera & Input & Components\n"
		<< "5) Demo 5 - Lighted Scene - introduction to Lights & Renderer3D & Techniques\n";

	char choice;
	std::cin >> choice;
	if (choice == '1') {
		Start();
		Demo1 *demo = new Demo1();
		Core::Engine::Run(demo);
	}
	else if (choice == '2') {
		Start();
		Demo2 demo;
		Core::Engine::Run(&demo);	
	}
	else if (choice == '3') {
		Start();
		Demo3 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == '4') {
		Start();
		Demo4 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == '5') {
		Start();
		Demo5 demo;
		Core::Engine::Run(&demo);
	}
	else {
		std::cout << "Wrong Choice\n";
		system("pause");
		return 0;

	}

	return 0;
}