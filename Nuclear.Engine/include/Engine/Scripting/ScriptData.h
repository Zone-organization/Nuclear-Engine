#pragma once
#include <Engine\Assets\Script.h>

class asIScriptObject;

namespace Nuclear
{
	namespace Scripting
	{
		typedef void* ScriptFunction;
		typedef asIScriptObject* ScriptObject;

		//Describes script data in single module
		class NEAPI ScriptData
		{
		public:
			ScriptData();
			~ScriptData();


			Assets::Script* mScriptParent = nullptr;
			ScriptFunction mLoadfun = nullptr;
			ScriptFunction mUpdateFun = nullptr;
			ScriptObject mObjectInstance = nullptr;
		};
	}
}