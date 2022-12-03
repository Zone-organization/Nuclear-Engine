#pragma once
#include "Engine\Scripting\ScriptingEngine.h"
#include "Engine\ECS\System.h"
#include <vector>

namespace Nuclear
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
		class NEAPI ScriptingSystem : public ECS::System<ScriptingSystem>
		{
		public:
			ScriptingSystem();
			~ScriptingSystem();

			bool Initialize();

			void Load() override;
			void Update(ECS::TimeDelta dt) override;
		private:
		};

	}
}