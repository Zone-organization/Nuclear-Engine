#pragma once
#include <NE_Common.h>
struct _MonoObject;

namespace Nuclear
{
	namespace Scripting
	{
		namespace Bindings
		{
			// Logger
			void Utilities_Logger_Trace(_MonoObject* message);
			void Utilities_Logger_Info(_MonoObject* message);
			void Utilities_Logger_Warn(_MonoObject* message);
			void Utilities_Logger_Error(_MonoObject* message);
			void Utilities_Logger_FatalError(_MonoObject* message);

			//Entity
			void ECS_Entity_AddComponent(Uint32 id, void* type);
			bool ECS_Entity_HasComponent(Uint32 id, void* type);
		}
	}
}
