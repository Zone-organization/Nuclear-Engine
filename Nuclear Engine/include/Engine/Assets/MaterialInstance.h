#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets\Texture.h>
#include <Engine\Assets\Material.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>

namespace NuclearEngine
{
	namespace Assets
	{
		//TODO: Support more Shader Types.
		struct MICreationDesc
		{
			Graphics::RenderingPipeline* mPipeline;
			RefCntAutoPtr<ISampler> mSampler;
		};

		struct MIShaderTexture
		{
			Uint32 mSlot = 0;
			Assets::Texture mTexture;
		};

		typedef	std::vector<MIShaderTexture> MITextureSet;

		class NEAPI MaterialInstance
		{
		public:
			MaterialInstance();
			~MaterialInstance();

			void Create(const MICreationDesc& desc);
			void Initialize(const std::vector<TextureSet>& PixelShaderTextures);

			void BindTexSet(Uint32 index);

			std::vector<MITextureSet> mPShaderTextures;
		private:
			Graphics::RenderingPipeline* mRenderingPipeline = nullptr;
			RefCntAutoPtr<IShaderResourceBinding> mSRB;
		};
	}
}