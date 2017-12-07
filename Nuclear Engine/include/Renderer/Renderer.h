#pragma once
#include <NE_Common.h>
#include <Components\GenericCamera.h>
#include <vector>

namespace NuclearEngine {
	namespace Components 
	{
		struct Light;
	}

	namespace Core {

		enum class NEAPI LightShading
		{
			PhongShading
		};

		enum class NEAPI LightEffects
		{
			None
		};

		enum class NEAPI RenderingTechnique
		{
			Forward
		};

		enum class Renderer3DStatusFlag
		{
			Ready,
			RequireBaking
		};

		struct NEAPI Renderer3D_Desc
		{
			//Light
			LightShading lightmodel;
			LightEffects effect;
			RenderingTechnique tech;
		};

		class NEAPI Renderer
		{
		public:
			Renderer(const Renderer3D_Desc& desc);

			void AddLight(Components::Light *light);

			API::Shader *GetShader();

			void SetCamera(Components::GenericCamera *cam);

			void SetLightGlobalAmbient(API::Color globalambient);
			void SetCamera(Components::GenericCamera *cam);

			void Bake();

			void Render();
		private:
			void Bake_Uniform_Buffers();
			void Bake_Forward_Phong();

			API::Shader *Renderer_Shader;
			API::UniformBuffer *NE_LightUBO;
			size_t LightUBOSize;

			std::vector<Components::Light*> Lights;

			Renderer3DStatusFlag flag;

			Renderer3D_Desc m_desc;
			Components::GenericCamera *m_cam;

			Math::Vector4 m_globalambient;

			bool bakedbefore;
			bool materialubochanged = false;
		};
	}
}