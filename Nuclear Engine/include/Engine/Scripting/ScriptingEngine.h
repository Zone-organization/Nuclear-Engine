#pragma once
#include <Base\NE_Common.h>
#include "Engine\Assets\Script.h"

namespace NuclearEngine
{
	namespace Systems
	{
		class ScriptingSystem;
	}
	namespace Scripting
	{
		class ScriptingModule;

		enum class ScriptModuleCreationFlags
		{
			ONLY_IF_EXISTS = 0,
			CREATE_IF_NOT_EXISTS = 1,
			ALWAYS_CREATE = 2
		};

		struct ScriptModuleCreationDesc
		{
			std::string mName;
			ScriptModuleCreationFlags mFlags;
		};

		class ScriptingEngine
		{
		public:
			virtual bool Initialize() = 0;

			virtual void Shutdown() = 0;

			virtual bool CreateScript(Assets::Script* script, const std::string& scriptcode, Scripting::ScriptingModule* scriptmodule) = 0;

			virtual void CreateScriptingModule(Scripting::ScriptingModule* scriptmodule, ScriptModuleCreationDesc desc) = 0;

		};
	}
}