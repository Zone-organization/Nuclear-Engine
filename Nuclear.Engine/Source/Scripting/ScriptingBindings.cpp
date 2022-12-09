#include "Scripting/ScriptingBindings.h"
#include "Scripting/ScriptingEngine.h"
#include "Assets/Scene.h"
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <Utilities/Logger.h>
#include <ECS/Entity.h>
#include <Core/Engine.h>
#include <Core/Client.h>
#include <Managers/SceneManager.h>

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
				ECS::Entity entity(Core::Engine::GetInstance().GetClient()->GetDefaultSceneManager()->GetActiveScene()->GetRegistry(), id);

				MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
				ScriptingEngine::GetInstance().GetRegistry().mAddComponentFuncs[monoType](entity);				
			}
			bool ECS_Entity_HasComponent(Uint32 id, void* type)
			{
				ECS::Entity entity(Core::Engine::GetInstance().GetClient()->GetDefaultSceneManager()->GetActiveScene()->GetRegistry(), id);

				MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
				bool result = ScriptingEngine::GetInstance().GetRegistry().mHasComponentFuncs[monoType](entity);
				return result;
			}
		
		}
	}
}