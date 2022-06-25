#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace Nuclear
{
	namespace Graphics
	{
		struct RenderTargetDesc
		{
			RenderTargetDesc();
			RenderTargetDesc(TEXTURE_FORMAT colorTexFormat, TEXTURE_FORMAT depthTexFormat, std::string name = "Unnamed RT");

			//Debug Only
			std::string mName;
			std::string mType;  

			Uint32 Width = 800;
			Uint32 Height = 600;
			TEXTURE_FORMAT ColorTexFormat = TEX_FORMAT_RGBA8_UNORM;

			//If it isnt equal unknown a depth RT will be created
			TEXTURE_FORMAT DepthTexFormat = TEX_FORMAT_UNKNOWN;

			OptimizedClearValue ClearValue;
		};

		class NEAPI RenderTarget
		{
		public:
			RenderTarget();
			~RenderTarget();

			void Create(const RenderTargetDesc& Desc);

			void Resize(Uint32 width, Uint32 height);

			//void SetActive(const float* RGBA);
			Uint32 GetWidth() const;
			Uint32 GetHeight() const;

			//Shader Resource View
			ITextureView* GetSRV();
			//Render Target View
			ITextureView* GetRTV();

			ITextureView** GetRTVDblPtr();

			RenderTargetDesc GetDesc() const;
		protected:
			void CreateViews();

			RefCntAutoPtr<ITextureView> mRTV;
			RefCntAutoPtr<ITextureView> mSRV;

			RenderTargetDesc mRTDesc;
		};
	}
}