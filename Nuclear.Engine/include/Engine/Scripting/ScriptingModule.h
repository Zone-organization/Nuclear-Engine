#pragma once
#include <Engine/Scripting/ScriptData.h>
#include <unordered_map>

struct _MonoAssembly;

namespace Nuclear
{
	namespace Scripting
	{
		//Script Scope "kinda" = MonoAssembly
		class ScriptingModule
		{
		public:

			std::unordered_map<Assets::Script*, ScriptData> mImportedScripts;
			_MonoAssembly* pAssembly;
		};
	}
}
