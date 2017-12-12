#pragma once
#include <Shading\Technique.h>
#include <Components\GenericCamera.h>

namespace NuclearEngine {
	namespace Renderers {

		class NEAPI Renderer3D 
		{
		public:
			static bool Initialize(Components::GenericCamera* camera);

			static void SetTechnique(Shading::Technique* Tech);

			static API::Shader* GetShader();

			static void Reload();

			static void Render_Light();
		};
	}
}