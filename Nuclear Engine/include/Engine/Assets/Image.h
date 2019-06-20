#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine {
	namespace Assets {
		
		class NEAPI Image
		{
		public:
			Uint32 mWidth = 0;

			Uint32 mHeight = 0;

			Uint32 mBitsPerPixel = 0;

			Uint32 mNumComponents = 0;

			Uint32 mRowStride = 0;

			Byte* mData = nullptr;
		};
	}
}