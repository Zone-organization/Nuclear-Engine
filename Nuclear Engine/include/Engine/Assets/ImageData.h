#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI ImageData
		{
		public:
			ImageData()
			{
				mWidth = 0;
				mHeight = 0;
				mBitsPerPixel = 0;
				mNumComponents = 0;
				mRowStride = 0;
				mData = nullptr;
			}
			~ImageData()
			{
				mWidth = 0;
				mHeight = 0;
				mBitsPerPixel = 0;
				mNumComponents = 0;
				mRowStride = 0;
				mData = nullptr;
			}

			Uint32 mWidth;

			Uint32 mHeight;

			Uint32 mBitsPerPixel;

			Uint32 mNumComponents;

			Uint32 mRowStride;

			Byte* mData;
		};
	}
}