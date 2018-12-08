#pragma once
#include <Core\ApplicationDesc.h>
#include <LLGL/LLGL.h>
#include <LLGL/Utility.h>

namespace NuclearEngine {
	namespace Core {

		class NEAPI Application
		{
		public:
			static bool Start(const ApplicationDesc& Desc);
			static void Shutdown();

			static void Display(bool show = true);

			static bool ShouldClose;

			static bool PollEvents();
			static void SetMouseInputMode(const MouseInputMode& mode);

			static Float32 GetAspectRatioF32();

			static LLGL::Window* GetWindow();
		};
	}
}
