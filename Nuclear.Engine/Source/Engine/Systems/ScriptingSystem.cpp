#include "Engine\Systems\ScriptingSystem.h"
#include "Engine\Scripting\ScriptingEngine.h"
#include "Engine\Components\ScriptComponent.h"
#include "Engine\Assets\Scene.h"
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Systems
	{
		ScriptingSystem::ScriptingSystem()
		{
		}
		ScriptingSystem::~ScriptingSystem()
		{
		}
		bool ScriptingSystem::Initialize()
		{

			

			NUCLEAR_ERROR("[ScriptingSystem] Failed to initialize...");
			return false;
		}

		void ScriptingSystem::Load()
		{
			auto view = mScene->GetRegistry().view<Components::ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<Components::ScriptComponent>(entity);

				script.mScriptObject.CallMethod(script.mScriptObject.GetScript()->mLoadMethod);
			}
		}

		void ScriptingSystem::Update(ECS::TimeDelta dt)
		{
			/*auto view = mScene->GetRegistry().view<Components::ScriptComponent>();

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
			}*/
		}

	}
}