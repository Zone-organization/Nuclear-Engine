#pragma once
#include <Base\NE_Common.h>
#include "Engine\Scripting\ScriptingEngine.h"
#include "Engine\ECS\System.h"
#include <vector>

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

		class NEAPI ScriptingSystem : public ECS::System<ScriptingSystem>
		{
		public:
			ScriptingSystem(ScriptingBackend backend = ScriptingBackend::AngelScript);
			~ScriptingSystem();

			bool Initialize();

			Scripting::ScriptingEngine* GetScriptingEngine();
			Scripting::ScriptingModule* CreateScriptingModule(const Scripting::ScriptModuleCreationDesc& desc);

			void Load() override;
			void Update(ECS::TimeDelta dt) override;
		private:
			Scripting::ScriptingEngine* mScriptEngine;
			std::vector<Scripting::ScriptingModule> _modules;
			ScriptingBackend mBackend;
		};

	}
}