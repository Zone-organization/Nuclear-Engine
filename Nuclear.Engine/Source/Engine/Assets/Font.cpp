#include <Engine/Assets/Font.h>

#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Assets
	{
		Font::Font()
		{
		}

		Font::~Font()
		{
		}
		void Font::Create(const FontCreationDesc& desc)
		{
			TextureDesc TexDesc;
			TexDesc.Type = RESOURCE_DIM_TEX_2D;
			TexDesc.Width = desc.AtlasWidth;
			TexDesc.Height = desc.AtlasHeight;
			TexDesc.MipLevels = 1;
			TexDesc.ArraySize = 1;
			TexDesc.Usage = USAGE_DEFAULT;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE;
			TexDesc.Format = TEX_FORMAT_R8_UNORM;
			TexDesc.CPUAccessFlags = CPU_ACCESS_NONE;

			TextureSubResData pSubResource;
			pSubResource.pData = desc.Data;
			pSubResource.Stride = desc.AtlasWidth;

			TextureData TexData;
			TexData.pSubResources = &pSubResource;
			TexData.NumSubresources = 1;
			RefCntAutoPtr<ITexture> mTexture;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(TexDesc, &TexData, &mTexture);

			mTextureView = mTexture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
		
		}
	}
}