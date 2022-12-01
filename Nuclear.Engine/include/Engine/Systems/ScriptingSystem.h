#pragma once
#include "Engine\Scripting\ScriptingEngine.h"
#include "Engine\Scripting\ScriptingModule.h"
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

			Scripting::ScriptingModule* CreateScriptingModule(const Scripting::ScriptModuleCreationDesc& desc);

			void Load() override;
			void Update(ECS::TimeDelta dt) override;
		private:
			std::vector<Scripting::ScriptingModule> _modules;
		};

	}
}