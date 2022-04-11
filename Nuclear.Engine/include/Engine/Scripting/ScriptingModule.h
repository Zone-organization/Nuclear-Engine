#pragma once
#include <Engine/Scripting/ScriptData.h>
#include <unordered_map>

namespace Nuclear
{
	namespace Scripting
	{
		//Script Scope "kinda"
		class ScriptingModule
		{
		public:

			std::unordered_map<Assets::Script*, ScriptData> mImportedScripts;
			void* ScriptingModulePtr;
		};
	}
}
