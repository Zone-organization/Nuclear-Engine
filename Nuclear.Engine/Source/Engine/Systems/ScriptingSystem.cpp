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
		void ScriptInitialization(entt::registry& reg, entt::entity entity)
		{
			auto& script = reg.get<Components::ScriptComponent>(entity);
			auto scriptasset = script.GetScriptAsset();
			if (scriptasset)
			{
				script.GetScriptingObject() = scriptasset->mClass.CreateObject();
				Uint32 entityID = static_cast<Uint32>(entity);
				void* param[] = { &entityID };
				script.GetScriptingObject().CallMethod(scriptasset->mConstructor, param);
				script.GetScriptingObject().CallMethod(scriptasset->mOnStartMethod);
			}


		}

		bool ScriptingSystem::Initialize()
		{
			// connects a free function
			mScene->GetRegistry().on_construct<Components::ScriptComponent>().connect<&ScriptInitialization>();

			

			NUCLEAR_INFO("[ScriptingSystem] ScriptingSystem initialized successfully...");
			return true;
		}

		void ScriptingSystem::Load()
		{
			//auto view = mScene->GetRegistry().view<Components::ScriptComponent>();

			//for (auto entity : view)
			//{
			//	auto& script = view.get<Components::ScriptComponent>(entity);

			//	//script.mScriptObject.CallMethod(script.mScriptObject.GetScript()->mLoadMethod);
			//}
		}

		void ScriptingSystem::Update(ECS::TimeDelta dt)
		{
			auto view = mScene->GetRegistry().view<Components::ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<Components::ScriptComponent>(entity);
				auto pScriptasset = script.GetScriptAsset();
				if (pScriptasset)
				{
					void* args[] = { &dt };
					script.GetScriptingObject().CallMethod(pScriptasset->mOnUpdateMethod, args);
				}

			}
		}

	}
}