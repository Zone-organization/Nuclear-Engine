#pragma once
#include <Engine\Assets\Mesh.h>
#include <Engine\Graphics\RenderTarget.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DepthStencilState.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct PostProcessDesc 
		{
			Uint8 Width;
			Uint8 Height;
			bool HDR;
			bool GammaCorrection;
			bool Bloom;
		};
		class CameraPostProcess
		{
		public:
			void Bake(const PostProcessDesc& desc);

			//Recreates the render target texture.
			void Resize();

		private:
			void CreateRenderTarget(const PostProcessDesc& Desc);
			void CreatePipeline(const PostProcessDesc& Desc);


			RefCntAutoPtr<ISampler> ScreenSampler;
			RenderTarget PostProcessRT;
			RefCntAutoPtr<IPipelineState> mPSO;
			Assets::Mesh PPScreenQuad;
		};
	}
}