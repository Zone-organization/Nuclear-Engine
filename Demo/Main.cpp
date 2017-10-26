#include "Common.h"
#define PTE_IMPLEMENTATION
#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"
#include "Demo6.h"

void Start()
{

	char choice;
	std::cout << "--Choose A Renderer: \nA) OpenGL 3 \nB) DirectX 11\n";
	std::cin >> choice;
	if (choice == 'a') {
		//Initialize Engine
		Core::Engine::Initialize(L"Prototype Engine", _Width_, _Height_);
		Core::Context::Initialize(Core::RenderAPI::OpenGL3);
	}
	else if (choice == 'b') {
		//Initialize Engine
		Core::Engine::Initialize(L"Prototype Engine", _Width_, _Height_);
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
		<< "--Select Low Level API Demos:\n"
		<< "A) Demo 1 - Introduction To Shaders & Vertex Buffers & Input Layouts\n"
		<< "B) Demo 2 - Introduction To Index Buffers & Textures\n"
		<< "C) Demo 3 - Introduction To Transformations & Constant Buffers\n"

		<< "--Select High Level Engine Demos:\n"
		<< "1) Demo 4 - Introduction To Camera & Input\n"
		<< "2) Demo 5 - Introduction To Lights & Renderer3D\n"
		<< "3) Demo 6 - Introduction To Model Loading\n";

	char choice;
	std::cin >> choice;
	if (choice == 'a') {
		Start();
		Demo1 *demo = new Demo1();
		Core::Engine::Run(demo);
	}
	else if (choice == 'b') {
		Start();
		Demo2 demo;
		Core::Engine::Run(&demo);	
	}
	else if (choice == 'c') {
		Start();
		Demo3 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == '1') {
		Start();
		Demo4 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == '2') {
		Start();
		Demo5 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == '3') {
		Start();
		Demo6 demo;
		Core::Engine::Run(&demo);
	}
	else {
		std::cout << "Wrong Choice\n";
		system("pause");
		return 0;

	}

	return 0;
}