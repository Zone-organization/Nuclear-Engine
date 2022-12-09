#include "Engine/Scripting/ScriptingBindings.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Assets/Scene.h"
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <Core\Logger.h>
#include <Engine/ECS/Entity.h>
#include <Engine.h>
#include <Client.h>
#include <Engine/Managers/SceneManager.h>

namespace Nuclear
{
	namespace Scripting
	{
		namespace Bindings
		{
			void Core_Logger_Trace(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_TRACE(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_Info(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_INFO(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_Warn(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_WARN(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_Error(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_ERROR(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_FatalError(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				CLIENT_FATAL(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void ECS_Entity_AddComponent(Uint32 id, void* type)
			{
				ECS::Entity entity(Engine::GetInstance().GetClient()->GetDefaultSceneManager()->GetActiveScene()->GetRegistry(), id);

				MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
				ScriptingEngine::GetInstance().GetRegistry().mAddComponentFuncs[monoType](entity);				
			}
			bool ECS_Entity_HasComponent(Uint32 id, void* type)
			{
				ECS::Entity entity(Engine::GetInstance().GetClient()->GetDefaultSceneManager()->GetActiveScene()->GetRegistry(), id);

				MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
				bool result = ScriptingEngine::GetInstance().GetRegistry().mHasComponentFuncs[monoType](entity);
				return result;
			}
		
		}
	}
}