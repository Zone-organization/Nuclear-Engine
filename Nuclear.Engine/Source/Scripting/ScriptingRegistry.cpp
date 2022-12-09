#include <Scripting/ScriptingRegistry.h>
#include <Scripting/ScriptingAssembly.h>
#include <Components/Components.h>
#include <Utilities/Logger.h>

#include <mono/metadata/reflection.h>

#define RegisterEngineComponent(Type, image)                                                                    \
    {                                                                                              \
        MonoType* monotype = mono_reflection_type_from_name((char*)"Nuclear.Components." #Type, image);     \
        if (monotype)                                                                                  \
        {                                                                                          \
            Uint32 id = mono_type_get_type(monotype);                                                    \
            mAddComponentFuncs[monotype] = [](ECS::Entity& entity) { entity.AddComponent<Type>(); };     \
           	mHasComponentFuncs[monotype] = [](ECS::Entity& entity) { return entity.HasComponent<Type>(); }; \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
           NUCLEAR_ERROR("[ScriptingRegistry] " #Type" has no C# equivalent");                     \
        }                                                                                          \
    }

namespace Nuclear
{
	namespace Scripting
	{
		ScriptingRegistry::ScriptingRegistry()
		{
			mAddComponentFuncs.clear();
			mHasComponentFuncs.clear();
		}
		ScriptingRegistry::~ScriptingRegistry()
		{
			mAddComponentFuncs.clear();
			mHasComponentFuncs.clear();
		}
		void ScriptingRegistry::RegisterEngineComponents(ScriptingAssembly* coreassembly)
		{
			using namespace Components;
			RegisterEngineComponent(EntityInfoComponent, coreassembly->GetImage());
			RegisterEngineComponent(CameraComponent, coreassembly->GetImage());
			RegisterEngineComponent(MeshComponent, coreassembly->GetImage());
			RegisterEngineComponent(ColliderComponent, coreassembly->GetImage());
			RegisterEngineComponent(RigidBodyComponent, coreassembly->GetImage());
			RegisterEngineComponent(DirLightComponent, coreassembly->GetImage());
			RegisterEngineComponent(PointLightComponent, coreassembly->GetImage());
			RegisterEngineComponent(SpotLightComponent, coreassembly->GetImage());
			RegisterEngineComponent(ScriptComponent, coreassembly->GetImage());
		}
	}
}