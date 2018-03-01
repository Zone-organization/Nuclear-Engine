#define NUCLEAR_ENGINE_MAIN_IMPLEMENTATION
#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Playground.h"
#include <iostream>

void Test();

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
		<< "----------------[Other]---------------\n"
		<< "x) Playground - Sandbox for testing new stuff\n"
		<< "z) Tests\n";


	char choice;
	std::cin >> choice;
	if (choice == '1') {
		Start();
		Sample1 demo;
		Core::Engine::Run(&demo);
	}
	if (choice == '2') {
		Start();
		Sample2 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == 'x') {
		Start();
		Playground game;
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
	else {
		std::cout << "Running Test...\n";
		Test();
		system("pause");
		return 0;

	}

	Core::Engine::ShutDown();

	return 0;
}

int z;
void Foo1()
{
	z = 0;
}
void Foo2()
{
	z = 1;
}
void Foo3()
{
	z = 1;
}void Foo4()
{
	z = 1;
}void Foo5()
{
	z = 1;
}

int i = 0;

void IFSTATEMENT()
{
	if (i == 0)
	{
		Foo1();
	}
	else if (i == 1)
	{
		Foo2();
	}
	else if (i == 2)
	{
		Foo3();
	}
	else if (i == 3)
	{
		Foo4();
	}
	else if (i == 4)
	{
		Foo5();
	}
	else
	{
		Foo1();
	}
}

void SWITCHCASE()
{
	switch (i)
	{
	case 1:
		Foo1();
	case 2:
		Foo2();
	case 3:
		Foo3();
	case 4:
		Foo4();
	case 5:
		Foo5();
	}
}

class Base {
public:
	virtual void CallMe()
	{
		Foo1();
	}
};

class Derived : public Base {
public:
	void CallMe() override
	{
		Foo2();
	}
};

void Test()
{
	NuclearEngine::Platform::Clock clock;
	Base base;
	Derived derived;
	Base *base2 = new Derived();

	std::cin >> i;

	std::cout << "Normal\n";
	clock.Restart();
	for (int i = 0; i < 10000000; i++)
	{
		Foo1();
	}
	std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;
	
	std::cout << "Switch Statement\n";
	clock.Restart();
	for (int i = 0; i < 10000000; i++)
	{
		SWITCHCASE();
	}
	std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;


	std::cout << "If Statement\n";
	clock.Restart();
	for (int i = 0; i < 10000000; i++)
	{
		IFSTATEMENT();
	}
	std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;
	
	std::cout << "Base\n";
	clock.Restart();
	for (int i = 0; i < 10000000; i++)
	{
		base.CallMe();
	}
	std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;

	std::cout << "Derived\n";
	clock.Restart();
	for (int i = 0; i < 10000000; i++)
	{
		derived.CallMe();
	}
	std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;

	std::cout << "New Derived\n";
	clock.Restart();
	for (int i = 0; i < 10000000; i++)
	{
		base2->CallMe();
	}
	std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;

}