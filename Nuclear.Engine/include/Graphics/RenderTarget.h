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
		struct RenderTargetDesc
		{
			RenderTargetDesc()
			{
				mName = "UnNamed";
				mType = "UnNamed";
			}

			RenderTargetDesc(Diligent::TEXTURE_FORMAT colorTexFormat, Diligent::TEXTURE_FORMAT depthTexFormat, std::string name = "Unnamed RT")
			{
				ColorTexFormat = colorTexFormat;
				DepthTexFormat = depthTexFormat;
				mName = name;
				mType = "UnNamed";
			}

			//Debug Only
			std::string mName;
			std::string mType;  

			Math::Vector2ui mDimensions = Math::Vector2ui(800, 600);
			Diligent::TEXTURE_FORMAT ColorTexFormat = Diligent::TEX_FORMAT_RGBA8_UNORM;

			//If it isnt equal unknown a depth RT will be created
			Diligent::TEXTURE_FORMAT DepthTexFormat = Diligent::TEX_FORMAT_UNKNOWN;

			Diligent::OptimizedClearValue ClearValue;
		};

		class NEAPI RenderTarget
		{
		public:
			RenderTarget();
			~RenderTarget();

			void Create(const RenderTargetDesc& Desc);

			void Release();

			void Resize(const Math::Vector2ui& dimensions);

			Math::Vector2ui GetDimensions() const;

			//Shader Resource View
			Diligent::ITextureView* GetSRV();
			//Render Target View
			Diligent::ITextureView* GetRTV();

			Diligent::ITextureView** GetRTVDblPtr();

			RenderTargetDesc GetDesc() const;
		protected:
			void CreateViews();

			Diligent::RefCntAutoPtr<Diligent::ITextureView> mRTV;
			Diligent::RefCntAutoPtr<Diligent::ITextureView> mSRV;

			RenderTargetDesc mDesc;
		};
	}
}