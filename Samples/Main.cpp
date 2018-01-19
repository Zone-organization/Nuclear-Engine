#define NUCLEAR_ENGINE_MAIN_IMPLEMENTATION
#include "Common.h"
#include "Sample1.h"
//#include "Demo5.h"
#include "Game_Breakout.h"
#include <iostream>

void Test();

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
		<< "-----[Select the sample you want]-----\n"

		<< "---High level engine demos:\n"
		<< "1) Sample 1 - Dynamic Textured Cube - introduction to Camera & Input & Components\n"
		<< "2) Sample 2 - Lighted Scene - introduction to Lights & Renderer3D & Techniques\n"

		<< "---Game demos:\n"
		<< "a) Breakout - 2D\n";

	char choice;
	std::cin >> choice;
	if (choice == '1') {
		Start();
		Sample1 demo;
		Core::Engine::Run(&demo);
	}
	else if (choice == 'a') {
		Start();
		BreakOut game;
		Core::Engine::Run(&game);
	}
	else {
		std::cout << "Running Test...\n";
		Test();
		system("pause");
		return 0;

	}

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