#pragma once
#include <Shading\Technique.h>
#include <Components\GenericCamera.h>
#include <Components\Light.h>
#include <vector>
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
			Renderer3D();
			~Renderer3D();

			void SetCamera(Components::GenericCamera* camera);
			void SetTechnique(Shading::Technique* Tech);

			API::Shader GetShader();

			void AddLight(Components::DirectionalLight* light);
			void AddLight(Components::PointLight* light);
			void AddLight(Components::SpotLight* light);

			void Bake();

			void Render_Light();

		private:
			void Calculate_Light_CB_Size();

			API::Shader Shader;
			API::ConstantBuffer NE_Light_CB;
			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;
			Shading::Technique* Light_Rendering_Tech;
			Components::GenericCamera* Camera;
			std::vector<Components::DirectionalLight*> DirLights;
			std::vector<Components::PointLight*> PointLights;
			std::vector<Components::SpotLight*> SpotLights;
			Renderer3DStatusFlag flag;
		};
	}
}