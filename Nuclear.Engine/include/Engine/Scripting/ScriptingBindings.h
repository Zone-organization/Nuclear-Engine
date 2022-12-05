#pragma once
struct _MonoObject;

namespace Nuclear
{
	namespace Scripting
	{
		namespace Bindings
		{
			// Logger
			void Core_Logger_Trace(_MonoObject* message);
			void Core_Logger_Info(_MonoObject* message);
			void Core_Logger_Warn(_MonoObject* message);
			void Core_Logger_Error(_MonoObject* message);
			void Core_Logger_FatalError(_MonoObject* message);
		}
	}
}
