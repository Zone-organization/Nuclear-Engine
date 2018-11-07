#pragma once
#include <Base\NE_Common.h>
#define DEFAULT_SFML_STYLE 7

namespace NuclearEngine {
	namespace Core {
		enum class MouseInputMode { Virtual, Normal, Hidden };

		enum class RenderAPI : Uint8
		{
			OpenGL4_5,
			DirectX11
		};

		struct ApplicationDesc
		{
			Uint32 WindowWidth = 1024;
			Uint32 WindowHeight = 768;
			Uint32 Style = DEFAULT_SFML_STYLE;
			RenderAPI Renderer = RenderAPI::OpenGL4_5;
			std::string Title = "NuclearEngine";
		};
	}
}
