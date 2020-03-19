#pragma once
#include <Base\NE_Common.h>
#include <Engine\Assets\MaterialTypes.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		class RenderingPipeline;
	}
	namespace Assets
	{
		class NEAPI MaterialInstance
		{
		public:
			MaterialInstance();
			~MaterialInstance();

			void Create(Graphics::RenderingPipeline* Pipeline);
			void Initialize(const std::vector<TextureSet>& PixelShaderTextures);

			void BindTexSet(Uint32 index);

			std::vector<TextureSet> mPShaderTextures;
		private:
			Graphics::RenderingPipeline* mRenderingPipeline = nullptr;
			RefCntAutoPtr<IShaderResourceBinding> mSRB;
		};
	}
}