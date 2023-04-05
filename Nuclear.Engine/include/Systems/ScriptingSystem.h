#pragma once
#include "Scripting\ScriptingModule.h"
#include "ECS\System.h"
#include <vector>

namespace Nuclear
{			
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