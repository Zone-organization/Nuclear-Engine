#pragma once
#include <NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DepthStencilState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace Nuclear
{
	namespace Components
	{
		class CameraComponent;
	}
	namespace Assets
	{
		class Texture;
	}
	namespace Rendering
	{
		class ImageBasedLighting;

		class NEAPI Skybox
		{
		public:
			Skybox();
			~Skybox();
			//TODO: Move
			void Initialize(const std::array<Assets::Texture*, 6>& data);     

			void Initialize(Assets::Texture* Texture);

			void Render();

		protected:
			friend class ImageBasedLighting;

			void InitializePipeline();
			void InitializeCube();
			void RenderCube();

			RefCntAutoPtr<ITextureView> mTextureSRV;
		private:
			RefCntAutoPtr<IPipelineState> mPipeline;
			RefCntAutoPtr<IBuffer> mVBuffer;
			RefCntAutoPtr<ISampler> mSampler;
			RefCntAutoPtr<IShaderResourceBinding> mSRB;
			bool ReleaseTex = false;
			bool Valid = false;
		};
	}
}