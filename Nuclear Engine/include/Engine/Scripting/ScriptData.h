#pragma once
#include <Engine\Assets\Script.h>

namespace NuclearEngine
{
	namespace Scripting
	{
		typedef void* ScriptFunction;
		//Describes script data in single module
		class NEAPI ScriptData
		{
		public:
			ScriptData();
			~ScriptData();


			Assets::Script* mScriptParent = nullptr;
			ScriptFunction mStartfun = nullptr;
			ScriptFunction mUpdateFun = nullptr;
		};
	}
}