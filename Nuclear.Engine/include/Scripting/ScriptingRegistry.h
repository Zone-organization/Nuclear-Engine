#pragma once
#include <NE_Common.h>
#include <ECS/Entity.h>
#include <Scripting/MonoDeclarations.h>
#include <functional>
#include <unordered_map>

namespace Nuclear
{
	namespace Scripting
	{
		class ScriptingAssembly;

		class NEAPI ScriptingRegistry
		{
		public:
			ScriptingRegistry();
			~ScriptingRegistry();

			void RegisterEngineComponents(ScriptingAssembly* coreassembly);

	//	protected:
			std::unordered_map<_MonoType*, std::function<bool(ECS::Entity&)>> mHasComponentFuncs;
			std::unordered_map<_MonoType*, std::function<void(ECS::Entity&)>> mAddComponentFuncs;
		};
	}
}
