#include "Engine/Rendering/DirShadowMap.h"
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Rendering
	{
		void DirShadowMap::Initialize(const ShadowMapDesc& Desc)
		{	
			//Create Color Texture
			TextureDesc TexDesc;
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = Desc.Width;
			TexDesc.Height = Desc.Height;
			TexDesc.MipLevels = 1;
			TexDesc.ClearValue = Desc.ClearValue;

			//Create Depth Texture
			RefCntAutoPtr<ITexture> pRTDepth;
			TexDesc.Format = Desc.DepthTexFormat;
			TexDesc.ClearValue.DepthStencil.Depth = 1;
			TexDesc.ClearValue.DepthStencil.Stencil = 0;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

			Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &pRTDepth);
			mDepthDSV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);
		}
	}
}