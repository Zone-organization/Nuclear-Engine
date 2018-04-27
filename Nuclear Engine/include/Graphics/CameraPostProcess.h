#pragma once
#include <Graphics/API/RenderAPI.h>
#include <Assets\Mesh.h>

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
			Graphics::API::Sampler ScreenSampler;
			Graphics::API::Texture PostProcessTexture;
			Graphics::API::RenderTarget PostProcessRT;
			Graphics::API::VertexShader PostProcess_VShader;
			Graphics::API::PixelShader PostProcess_PShader;
			Assets::Mesh PostProcessScreenQuad;
		};
	}
}