#pragma once
#include <Engine/Graphics/API/RenderAPI.h>
#include <Engine\Assets\Mesh.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct PostProcessDesc 
		{
			Uint8 WindowWidth;
			Uint8 WindowHeight;
			bool HDR;
			bool GammaCorrection;
			bool Bloom;
		};
		class CameraPostProcess
		{
		public:
			void Bake(const PostProcessDesc& desc);
			LLGL::Sampler ScreenSampler;
			LLGL::Texture* PostProcessTexture;
			LLGL::RenderTarget PostProcessRT;
			LLGL::Shader* PostProcess_VShader;
			LLGL::Shader* PostProcess_PShader;
			Assets::Mesh PostProcessScreenQuad;
		};
	}
}