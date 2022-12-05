#pragma once
#include <Core/NE_Common.h>
#include <Engine/Scripting/MonoDeclarations.h>
#include <string>


namespace Nuclear
{
	namespace Scripting
	{
		//Script Scope "kinda" = MonoAssembly = DLL
		//Engine DLL :  Nuclear.ScriptCore, Client DLL : ClientScripts.dll
		class NEAPI ScriptingAssembly
		{
		public:
			ScriptingAssembly();

			//Namespace should be included "Nuclear.ScriptCore:Load()"
			ScriptFunction GetMethod(const std::string& methodname);

		//	std::unordered_map<Assets::Script*, ScriptData> mImportedScripts;
			_MonoAssembly* pAssembly;
			_MonoImage* pImage;
		};
	}
}
