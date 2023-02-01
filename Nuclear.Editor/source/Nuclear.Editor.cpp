// Nuclear.Editor.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <Nuclear.Editor.h>

#pragma comment(lib,"Nuclear.Engine.lib")
#pragma comment(lib,"Nuclear.Core.lib")
//#pragma comment(lib,"glfw3.lib")

int main()
{
	Core::EngineStartupDesc desc;
	desc.mEngineWindowDesc.WindowWidth = 1280;
	desc.mEngineWindowDesc.WindowHeight = 720;
	desc.mScriptingClientDllName = "EditorClient.dll";
	desc.mScriptingAssemblyNamespace = "Editor";
	desc.mAssetsLibraryPath = "../Assets/";
	Core::Engine::GetInstance().Start(desc);

	Core::Path::mReservedPaths["@EditorAssets@"] = "../Assets/NuclearEditor";
	Core::Path::mReservedPaths["@CommonAssets@"] = "../Assets/Common";

	while (!Core::Engine::GetInstance().ShouldClose())
	{
		NuclearEditor editor;
		Core::Engine::GetInstance().LoadClient(&editor);
	}
	Core::Engine::GetInstance().EndClient();
	return 0;
}
