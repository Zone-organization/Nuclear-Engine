#include "Common.h"
#include "Sample1.h"
#include <iostream>


int main(int argc, char* argv[])
{
	Core::EngineStartupDesc desc;

	desc.mAppdesc.AutoSelectRenderer = false;
	Core::Engine::Start(desc);
	Sample1 demo;
	Core::Engine::RunGame(&demo);
	
	Core::Engine::Shutdown();

	return 0;
}