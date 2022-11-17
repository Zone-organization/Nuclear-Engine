//#pragma once
//#include <Engine\Rendering\ShadingModel.h>
//#include <Engine\Rendering\ImageBasedLighting.h>
//#include <vector>
//
//namespace Nuclear
//{
//	namespace Rendering
//	{
//
//		class NEAPI PBR : public ShadingModel
//		{
//		public:
//			PBR(ImageBasedLighting* IBLContext = nullptr);
//
//			void ReflectPixelShaderData() override;
//
//			bool Bake(const ShadingModelBakingDesc& desc) override;
//			std::vector<Graphics::RenderTargetDesc> GetGBufferDesc() override;
//		private:
//			void BakeGBufferPipeline(const ShadingModelBakingDesc& desc);
//			ImageBasedLighting* pIBLContext;
//		};
//
//	}
//}