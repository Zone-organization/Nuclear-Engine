#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/Scripting/ScriptingModule.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <Core\Logger.h>
#pragma comment(lib,"mono-2.0-sgen.lib")


namespace Nuclear
{
	namespace Scripting
	{
		bool ScriptingEngine::Initialize(const ScriptingEngineDesc& desc)
		{
			std::string lib = desc.mMonoRuntimeDir.GetRealPath() + "/lib";
			std::string etc = desc.mMonoRuntimeDir.GetRealPath() + "/etc";

			mono_set_dirs(lib.c_str(), etc.c_str());

			pRuntimeDomain = mono_jit_init("Scripting");
			
			if (!pRuntimeDomain)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to initialize pRuntimeDomain...");
				return false;
			}

			NUCLEAR_INFO("[ScriptingEngine] Mono has been initalized succesfully!");
			return true;
		}
		void ScriptingEngine::Shutdown()
		{
			mono_jit_cleanup(pRuntimeDomain);
		}
		bool ScriptingEngine::CreateScriptingModule(Scripting::ScriptingModule* scriptmodule, const ScriptModuleCreationDesc& desc)
		{
			scriptmodule->pAssembly = mono_domain_assembly_open(pRuntimeDomain, desc.mPath.GetRealPath().c_str());


			if (!scriptmodule->pAssembly)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to open Assembly %s..." , desc.mPath.GetInputPath());

				return false;
			}

		}
		ScriptingEngine::ScriptingEngine()
		{
			pRuntimeDomain = nullptr;
		}
		ScriptingEngine& ScriptingEngine::GetInstance()
		{
			static ScriptingEngine instance;

			return instance;
		}
	}
}