#include <Engine/Scripting/ScriptingEngine.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <Core\Logger.h>
#pragma comment(lib,"lib/mono-2.0-sgen.lib")


namespace Nuclear
{
	namespace Scripting
	{
		bool ScriptingEngine::Initialize()
		{
			pRuntimeDomain = mono_jit_init("Scripting");

			if (!pRuntimeDomain)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to initialize pRuntimeDomain...");
				return false;
			}
			return true;
		}
		void ScriptingEngine::Shutdown()
		{
			mono_jit_cleanup(pRuntimeDomain);
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