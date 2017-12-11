#pragma once
#include <Shading\Technique.h>

namespace NuclearEngine {
	namespace Renderers {

		class NEAPI Renderer3D 
		{
			static bool Initialize();

			static bool SetTechnique(Shading::Technique* Tech);

			static void Reload();

			static void Render_Light();
		};
	}
}