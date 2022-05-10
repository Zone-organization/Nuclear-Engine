#pragma once
#include <Engine\Assets\MaterialTypes.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace Nuclear
{
	namespace Rendering
	{
		class ShadingModel;
	}
	namespace Assets
	{
		class NEAPI MaterialInstance
		{
		public:
			MaterialInstance();
			~MaterialInstance();

			void Create(Rendering::ShadingModel* Pipeline);
			void Initialize(const std::vector<TextureSet>& PixelShaderTextures);

			void BindTexSet(Uint32 index);

			std::vector<TextureSet> mPShaderTextures;
		private:
			Rendering::ShadingModel* mRenderingPipeline = nullptr;
			RefCntAutoPtr<IShaderResourceBinding> mSRB;
			bool mDefferedMaterial = false;
		};
	}
}