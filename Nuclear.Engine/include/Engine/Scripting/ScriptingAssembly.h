#pragma once
#include <Core/NE_Common.h>
#include <string>
struct _MonoAssembly;
struct _MonoImage;
struct _MonoMethod;

namespace Nuclear
{
	namespace Scripting
	{
		//Script Scope "kinda" = MonoAssembly = DLL
		//Engine DLL :  Nuclear.ScriptCore, Client DLL : ClientScripts.dll
		class NEAPI ScriptingAssembly
		{
		public:
			_MonoMethod* GetMethod(const std::string& methodname);

		//	std::unordered_map<Assets::Script*, ScriptData> mImportedScripts;
			_MonoAssembly* pAssembly;
			_MonoImage* pImage;
		};
	}
}
