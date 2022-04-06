#include "Engine\Systems\ScriptingSystem.h"
#include "..\Source\Engine\Scripting\AngelScriptEngine\AngelScriptEngine.h" 

namespace NuclearEngine
{
	namespace Systems
	{
		ScriptingSystem::ScriptingSystem(ScriptingBackend backend)
		{
			mBackend = backend;
		}
		ScriptingSystem::~ScriptingSystem()
		{
		}
		bool ScriptingSystem::Initialize()
		{

			if (mBackend == ScriptingBackend::AngelScript)
			{
				mScriptEngine = new Scripting::Internal::AngelScriptEngine();

			}
			if (mScriptEngine->Initialize())
			{
				return true;
			}

			Log.Error("[ScriptingSystem] Failed to initialize...\n");
			return false;
		}

		Scripting::ScriptingEngine* ScriptingSystem::GetScriptingEngine()
		{
			return mScriptEngine;
		}

		void ScriptingSystem::Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt)
		{
		}

	}
}