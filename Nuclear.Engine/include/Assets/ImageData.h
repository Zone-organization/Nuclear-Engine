#pragma once
#include <NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent\Graphics\GraphicsTools\interface\GraphicsUtilities.h>
#include <string>
#include <vector>
namespace Nuclear
{
	namespace Assets
	{
		struct ImageDesc
		{
		public:
			ImageDesc()
			{
				mPath = "";
				mWidth = 0;
				mHeight = 0;
				mBitsPerPixel = 0;
				mNumComponents = 0;
				mRowStride = 0;
				mData = nullptr;
				mComponentType = Diligent::VT_UNDEFINED;
				mUsage = Diligent::USAGE_IMMUTABLE;
				mBindFlags = Diligent::BIND_SHADER_RESOURCE;
				mMipLevels = 0;
				mCPUAccessFlags = Diligent::CPU_ACCESS_NONE;
				mIsSRGB = false;
				mFormat = Diligent::TEX_FORMAT_UNKNOWN;
			}
			~ImageDesc()
			{
				mPath = "";
				mWidth = 0;
				mHeight = 0;
				mBitsPerPixel = 0;
				mNumComponents = 0;
				mRowStride = 0;
				mData = nullptr;
				mComponentType = Diligent::VT_UNDEFINED;
			}

			std::string mPath;

			Uint32 mWidth;

			Uint32 mHeight;

			Uint32 mBitsPerPixel;

			Uint32 mNumComponents;

			Uint32 mRowStride;

			Diligent::VALUE_TYPE mComponentType;

			Byte* mData;

			//DESC
			Diligent::USAGE mUsage;

			Diligent::BIND_FLAGS mBindFlags;

			Diligent::CPU_ACCESS_FLAGS mCPUAccessFlags;

			Diligent::RESOURCE_DIMENSION mType;

			Diligent::TEXTURE_FORMAT mFormat;

			bool mGenerateMipMaps = true;

			Uint32 mMipLevels;

			bool mIsSRGB;

			float mMipMapsAlphaCutoff = 0;

			Diligent::MIP_FILTER_TYPE mMipMapsFilter = Diligent::MIP_FILTER_TYPE_DEFAULT;
		};

		struct ImageData
		{
			ImageData()
			{
				mTexDesc = Diligent::TextureDesc();
				mSubresources = std::vector<Diligent::TextureSubResData>();
				mMips = std::vector< std::vector<Uint8> >();
			}
			Diligent::TextureDesc mTexDesc;
			std::vector<Diligent::TextureSubResData> mSubresources;
			std::vector< std::vector<Uint8> > mMips;
		};

	}
}