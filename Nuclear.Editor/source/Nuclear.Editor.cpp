// Nuclear.Editor.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <Nuclear.Editor.h>

#pragma comment(lib,"Nuclear.Engine.lib")
#pragma comment(lib,"Nuclear.Core.lib")
//#pragma comment(lib,"glfw3.lib")

int main()
{
	EngineStartupDesc desc;
	desc.mEngineWindowDesc.WindowWidth = 1280;
	desc.mEngineWindowDesc.WindowHeight = 720;

	Engine::GetInstance()->Start(desc);
	Core::Path::mReservedPaths["@CommonAssets@"] = "Assets/Common";
	Core::Path::mReservedPaths["@NuclearAssets@"] = "Assets/NuclearEngine";
	Core::Path::mReservedPaths["@EditorAssets@"] = "Assets/NuclearEditor";

	while (!Engine::GetInstance()->ShouldClose())
	{
		NuclearEditor editor;
		Engine::GetInstance()->LoadClient(&editor);
	}
	Engine::GetInstance()->EndClient();
	return 0;
}
