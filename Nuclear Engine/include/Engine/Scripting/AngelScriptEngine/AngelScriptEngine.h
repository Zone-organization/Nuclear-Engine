#pragma once

#ifdef EXPOSE_ANGELSCRIPT_ENGINE

#include <Base\NE_Common.h>
#include "Engine\Assets\Script.h"
#include "Engine\Scripting\ScriptingEngine.h"
#include "Engine\Scripting\ScriptingModule.h"
#include "AngelScriptingContext.h"

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;

namespace NuclearEngine
{
	namespace Scripting
	{
		namespace Internal
		{
			class NEAPI AngelScriptEngine : public ScriptingEngine
			{
			public:
				bool Initialize() override;

				void Shutdown() override;

				bool CreateScript(Assets::Script* script, const std::string& scriptcode, Scripting::ScriptingModule* scriptmodule) override;
				
				void CreateScriptingModule(Scripting::ScriptingModule* scriptmodule,  ScriptModuleCreationDesc desc) override;

				bool BuildScriptingModule(Scripting::ScriptingModule* scriptmodule) override;

				ScriptingContext* GetContext() override;

			private:
				AngelScriptingContext MainContext;
			};
		}
	}
}

#endif