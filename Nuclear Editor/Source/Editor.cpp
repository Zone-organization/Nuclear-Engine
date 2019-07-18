#include "Sample1.h"

namespace NuclearEditor
{
	void Start()
	{
		Core::EngineStartupDesc desc;
		Core::Engine::Start(desc);
		Sample1 demo;
		Core::Engine::RunGame(&demo);
		Core::Engine::Shutdown();
	}
}