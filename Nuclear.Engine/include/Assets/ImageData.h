#pragma once
#include <NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
namespace Nuclear
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
				mComponentType = VT_UNDEFINED;
			}
			~ImageData()
			{
				mWidth = 0;
				mHeight = 0;
				mBitsPerPixel = 0;
				mNumComponents = 0;
				mRowStride = 0;
				mData = nullptr;
				mComponentType = VT_UNDEFINED;
			}

			std::string mPath;

			Uint32 mWidth;

			Uint32 mHeight;

			Uint32 mBitsPerPixel;

			Uint32 mNumComponents;

			Uint32 mRowStride;

			VALUE_TYPE mComponentType;

			Byte* mData;
		};
	}
}