#pragma once
#include <Engine/Components/Light.h>
#include <LLGL/RenderTarget.h>
#include <vector>

namespace NuclearEngine
{
	namespace Graphics
	{
		class ShadowMap
		{
		public:
			ShadowMap();
			~ShadowMap();

			void Initialize(Uint8 Width, Uint8 Height);
			void Update(std::vector<Components::DirectionalLight*> DirLights,
						std::vector<Components::PointLight*> PointLights,
						std::vector<Components::SpotLight*> SpotLights);

			LLGL::RenderTarget* mDepthMap;

		private:
			Uint8 mWidth;
			Uint8 mHeight;
		};
	}
}