#pragma once
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
			LLGL::RenderTarget* PostProcessRT;
			Graphics::Shader PostProcess_VShader;
			Graphics::Shader PostProcess_PShader;
			Assets::Mesh PostProcessScreenQuad;
		};
	}
}