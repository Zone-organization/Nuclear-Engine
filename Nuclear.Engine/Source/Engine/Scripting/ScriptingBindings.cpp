#include "Engine/Scripting/ScriptingBindings.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Scripting
	{
		namespace Bindings
		{
			void Core_Logger_Trace(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				NUCLEAR_TRACE(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_Info(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				NUCLEAR_INFO(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_Warn(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				NUCLEAR_WARN(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_Error(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				NUCLEAR_ERROR(ScriptingEngine::GetInstance().ToStdString(str));
			}
			void Core_Logger_FatalError(_MonoObject* message)
			{
				MonoString* str = mono_object_to_string(message, NULL);
				NUCLEAR_FATAL(ScriptingEngine::GetInstance().ToStdString(str));
			}
		}
	}
}