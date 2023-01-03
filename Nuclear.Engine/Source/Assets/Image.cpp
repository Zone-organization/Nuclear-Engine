#include <Assets/Image.h>
#include <Graphics\Context.h>
#include "..\ThirdParty\DiligentTextureLoader\src\TextureLoader.h"
#include <Assets/Importer.h>
#include <chrono>

namespace Nuclear
{
	namespace Assets 
	{
		Image::Image()
			: IAsset(AssetType::Image)
		{
			mWidth = 0;
			mHeight = 0;
			mTextureView = nullptr;
		}

		Image::~Image()
		{
		}

		void Image::SetTextureView(ITextureView* view)
		{
			mTextureView = view;
		}
		ITextureView* Image::GetTextureView()
		{
			return mTextureView.RawPtr();
		}
		const Uint32 Image::GetWidth() const
		{
			return mWidth;
		}
		const Uint32 Image::GetHeight() const
		{
			return mHeight;
		}
	}
}