#include "Scripting/ScriptingBindings.h"
#include "Scripting/ScriptingEngine.h"
#include "Core/Scene.h"
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <Utilities/Logger.h>
#include <ECS/Entity.h>
#include <Core/Engine.h>
#include <Components/LightComponent.h>

namespace Nuclear
{
	namespace Scripting
	{
		namespace Bindings
		{
			void Utilities_Logger_Trace(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_TRACE(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Utilities_Logger_Info(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_INFO(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Utilities_Logger_Warn(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_WARN(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Utilities_Logger_Error(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_ERROR(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Utilities_Logger_FatalError(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_FATAL(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void ECS_Entity_AddComponent(Uint32 id, void* type)
			{
				ECS::Entity entity(Core::Scene::GetInstance().GetRegistry(), id);

				MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
				ScriptingEngine::GetInstance().GetRegistry().mAddComponentFuncs[monoType](entity);				
			}
			bool ECS_Entity_HasComponent(Uint32 id, void* type)
			{
				ECS::Entity entity(Core::Scene::GetInstance().GetRegistry(), id);

				MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
				bool result = ScriptingEngine::GetInstance().GetRegistry().mHasComponentFuncs[monoType](entity);
				return result;
			}

			bool Platform_Input_IsKeyPressed(Platform::Input::KeyCode key)
			{
				return Platform::Input::GetInstance().IsKeyPressed(key);
			}

			void Components_LightComponent_GetColor(Uint32 id, Graphics::Color* outcolor)
			{
				ECS::Entity entity(Core::Scene::GetInstance().GetRegistry(), id);
				*outcolor = entity.GetComponent<Components::LightComponent>().GetColor();
			}

			void Components_LightComponent_SetColor(Uint32 id, Graphics::Color* incolor)
			{
				ECS::Entity entity(Core::Scene::GetInstance().GetRegistry(), id);
				entity.GetComponent<Components::LightComponent>().SetColor(*incolor);
			}

			float Components_LightComponent_GetIntensity(Uint32 id)
			{
				ECS::Entity entity(Core::Scene::GetInstance().GetRegistry(), id);
				return entity.GetComponent<Components::LightComponent>().GetIntensity();
			}

			void Components_LightComponent_SetIntensity(Uint32 id, float inIntensity)
			{
				ECS::Entity entity(Core::Scene::GetInstance().GetRegistry(), id);
				entity.GetComponent<Components::LightComponent>().SetIntensity(inIntensity);
			}
		
		}
	}
}