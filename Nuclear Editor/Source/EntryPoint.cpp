#include "..\Include\EntryPoint.h"
//#include "..\Samples\Sample1.h"
#include <NuclearEngine.h>
using namespace NuclearEngine;

void Start()
{
	Core::EngineStartupDesc desc;
	Core::Engine::Start(desc);
	//Sample1 demo;
	//Core::Engine::RunGame(&demo);
	Core::Engine::Shutdown();
}
