#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\ECS\Scene.h>
#include "Engine\Scripting\ScriptingEngine.h"

namespace NuclearEngine
{
	//namespace Scripting
	//{
	//	namespace Internal
	//	{
	//		class AngelScriptEngine;
	//	}
	//}
			
	namespace Systems
	{
		enum class ScriptingBackend
		{
			AngelScript           //Only one working
			//CSharp
		};

		class NEAPI ScriptingSystem : public ECS::System<ScriptingSystem> {
		public:
			ScriptingSystem(ScriptingBackend backend = ScriptingBackend::AngelScript);
			~ScriptingSystem();

			bool Initialize();

			Scripting::ScriptingEngine* GetScriptingEngine();

			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;
		private:
			Scripting::ScriptingEngine* mScriptEngine;
			ScriptingBackend mBackend;
		};

	}
}