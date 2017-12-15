#pragma once
#include <Shading\Technique.h>
#include <Components\GenericCamera.h>
#include <Components\Light.h>

namespace NuclearEngine {
	namespace Renderers {

		enum class Renderer3DStatusFlag
		{
			Ready,
			RequireBaking
		};

		class NEAPI Renderer3D 
		{
		public:
			Renderer3D(Components::GenericCamera* camera);
			~Renderer3D();

			void SetTechnique(Shading::Technique* Tech);

			API::Shader* GetShader();

			void AddLight(Components::Light* light);

			void Bake();

			void Render_Light();

		private:
			API::Shader *Shader;
			API::UniformBuffer *NE_LightUBO;
			size_t LightUBOSize;
			Shading::Technique* Light_Rendering_Tech;
			Components::GenericCamera* Camera;
			std::vector<Components::Light*> Lights;
			Renderer3DStatusFlag flag;

		};
	}
}