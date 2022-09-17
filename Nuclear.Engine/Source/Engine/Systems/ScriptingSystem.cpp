#include "Engine\Systems\ScriptingSystem.h"
#define EXPOSE_ANGELSCRIPT_ENGINE
#include <Engine\Scripting\AngelScriptEngine\AngelScriptEngine.h>
#include <Engine\Scripting\AngelScriptEngine\AngelScriptingContext.h>
#include "Engine\Components\ScriptComponent.h"
#include <Engine\Assets\Scene.h>
#include "..\Source\ThirdParty\angelscript\include\angelscript.h" 
#include <Core\Logger.h>

namespace Nuclear
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

			NUCLEAR_ERROR("[ScriptingSystem] Failed to initialize...");
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

		void ScriptingSystem::Load()
		{
			auto view = mScene->GetRegistry().view<Components::ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<Components::ScriptComponent>(entity);
				for (auto it : _modules)
				{
					for (auto it2 : it.mImportedScripts)
					{
						GetScriptingEngine()->GetContext()->Prepare(it.mImportedScripts[it2.first].mLoadfun);
						GetScriptingEngine()->GetContext()->SetObject(it.mImportedScripts[it2.first].mObjectInstance);
						GetScriptingEngine()->GetContext()->Execute();
					}
				}
			}
		}

		void ScriptingSystem::Update(ECS::TimeDelta dt)
		{
			auto view = mScene->GetRegistry().view<Components::ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<Components::ScriptComponent>(entity);
				for (auto it : _modules)
				{
					for (auto it2 : it.mImportedScripts)
					{
						GetScriptingEngine()->GetContext()->Prepare(it.mImportedScripts[it2.first].mUpdateFun);
						GetScriptingEngine()->GetContext()->SetObject(it.mImportedScripts[it2.first].mObjectInstance);
						GetScriptingEngine()->GetContext()->SetArgFloat(0, dt);
						GetScriptingEngine()->GetContext()->Execute();
					}
				}
			}
		}

	}
}