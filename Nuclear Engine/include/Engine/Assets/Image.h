#pragma once
#include <Base\Math\Math.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Engine/Assets/Common.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI ImageData
		{
		public:
			Uint32 mWidth = 0;

			Uint32 mHeight = 0;

			Uint32 mBitsPerPixel = 0;

			Uint32 mNumComponents = 0;

			Uint32 mRowStride = 0;

			Byte* mData = nullptr;
		};
		class NEAPI Image : public Asset<Image>
		{
		public:
			Image();
			~Image();

			ImageData mData;
			RefCntAutoPtr<ITextureView> mTextureView;
		};
	}
}