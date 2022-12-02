#pragma once
#include <Engine\Assets\Script.h>

struct _MonoMethod;

namespace Nuclear
{
	namespace Scripting
	{
		typedef _MonoMethod* ScriptFunction;

		//Describes script data in single module
		class NEAPI ScriptData
		{
		public:
			ScriptData();
			~ScriptData();


			Assets::Script* mScriptParent = nullptr;
			ScriptFunction mLoadfun = nullptr;
			ScriptFunction mUpdateFun = nullptr;
		//	ScriptObject mObjectInstance = nullptr;
		};
	}
}