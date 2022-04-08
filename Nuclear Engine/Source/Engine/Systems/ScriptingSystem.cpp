#include "Engine\Systems\ScriptingSystem.h"
#define EXPOSE_ANGELSCRIPT_ENGINE
#include <Engine\Scripting\AngelScriptEngine\AngelScriptEngine.h>
#include "Engine\Components\ScriptComponent.h"

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

		Scripting::ScriptingModule* ScriptingSystem::CreateScriptingModule(const Scripting::ScriptModuleCreationDesc& desc)
		{
			Scripting::ScriptingModule result;
			GetScriptingEngine()->CreateScriptingModule(&result, desc);

			_modules.push_back(result);
			
			return &_modules.back();
		}

		void ScriptingSystem::Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt)
		{
			//ECS::ComponentHandle<Components::ScriptComponent> script;
			//for (ECS::Entity entity : es.entities_with_components(script))
			//{
				for (auto it : _modules)
				{
					for (auto it2 : it.mImportedScripts)
					{
						GetScriptingEngine()->GetContext()->Prepare(it.mImportedScripts[it2.first].mStartfun);
						GetScriptingEngine()->GetContext()->Execute();
						GetScriptingEngine()->GetContext()->UnPrepare();
					}
				}
			//}
		}

	}
}