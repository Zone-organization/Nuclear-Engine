#pragma once
#include <NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Math/NETypes.h>

namespace Nuclear
{
	namespace Graphics
	{
		struct NEAPI RenderTargetDesc
		{
			RenderTargetDesc();
			RenderTargetDesc(TEXTURE_FORMAT colorTexFormat, TEXTURE_FORMAT depthTexFormat, std::string name = "Unnamed RT");

			//Debug Only
			std::string mName;
			std::string mType;  

			Math::Vector2ui mDimensions = Math::Vector2ui(800, 600);
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

			void Resize(const Math::Vector2ui& dimensions);

			Math::Vector2ui GetDimensions() const;

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

			RenderTargetDesc mDesc;
		};
	}
}