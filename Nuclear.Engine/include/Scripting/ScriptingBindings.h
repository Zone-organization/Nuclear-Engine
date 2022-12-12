#pragma once
#include <NE_Common.h>
#include <Platform/Input.h>
#include <Graphics/Color.h>

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

			//Input
			bool Platform_Input_IsKeyPressed(Platform::Input::KeyCode key);

			//--------Components--------
			//LightComponent
			void Components_LightComponent_GetColor(Uint32 id, Graphics::Color* outcolor);
			void Components_LightComponent_SetColor(Uint32 id, Graphics::Color* incolor);

			float Components_LightComponent_GetIntensity(Uint32 id);
			void Components_LightComponent_SetIntensity(Uint32 id, float inIntensity);

		}
	}
}
