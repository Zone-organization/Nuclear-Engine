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
		}
	}
}