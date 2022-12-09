#pragma once
#include <Core/NE_Common.h>
#include <Engine/ECS/Entity.h>
#include <Engine/Scripting/MonoDeclarations.h>
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
