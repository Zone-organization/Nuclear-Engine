#include <Assets/Texture.h>
#include <Graphics/GraphicsModule.h>
#include "..\ThirdParty\DiligentTextureLoader\src\TextureLoader.h"
#include <Assets/Importer.h>
#include <chrono>

namespace Nuclear
{
	namespace Assets 
	{
		Texture::Texture()
			: IAsset(AssetType::Texture)
		{
			mWidth = 0;
			mHeight = 0;
			mTextureView = nullptr;
		}

		Texture::~Texture()
		{
		}

		void Texture::SetTextureView(Diligent::ITextureView* view)
		{
			mTextureView = view;
		}
		Diligent::ITextureView* Texture::GetTextureView()
		{
			return mTextureView.RawPtr();
		}
		const Uint32 Texture::GetWidth() const
		{
			return mWidth;
		}
		const Uint32 Texture::GetHeight() const
		{
			return mHeight;
		}
	}
}