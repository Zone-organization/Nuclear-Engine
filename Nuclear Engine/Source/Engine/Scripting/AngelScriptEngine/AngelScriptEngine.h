#pragma once
#include <Base\NE_Common.h>
#include "Engine\Assets\Script.h"
#include "Engine\Scripting\ScriptingEngine.h"
#include "Engine\Scripting\ScriptingModule.h"

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;

namespace NuclearEngine
{
	namespace Scripting
	{
		namespace Internal
		{
			class AngelScriptEngine : public ScriptingEngine
			{
			public:
				bool Initialize() override;

				void Shutdown() override;

				bool CreateScript(Assets::Script* script, const std::string& scriptcode, Scripting::ScriptingModule* scriptmodule) override;
				
				void CreateScriptingModule(Scripting::ScriptingModule* scriptmodule,  ScriptModuleCreationDesc desc) override;

			private:
			//	asIScriptModule* MainModule;
				//asIScriptContext* MainContext;
			};
		}
	}
}