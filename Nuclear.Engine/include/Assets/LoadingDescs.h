#pragma once
#include <Utilities/DataBlob.h>
#include <Core/Path.h>
#include <string>

namespace Nuclear
{
	namespace Assets
	{
		struct AssetMetadata;

		//FREE_IMAGE_FORMAT
		enum IMAGE_EXTENSION
		{
			IMAGE_EXTENSION_UNKNOWN = -1,
			IMAGE_EXTENSION_BMP = 0,
			IMAGE_EXTENSION_ICO = 1,
			IMAGE_EXTENSION_JPEG = 2,
			IMAGE_EXTENSION_JNG = 3,
			IMAGE_EXTENSION_KOALA = 4,
			IMAGE_EXTENSION_LBM = 5,
			IMAGE_EXTENSION_IFF = IMAGE_EXTENSION_LBM,
			IMAGE_EXTENSION_MNG = 6,
			IMAGE_EXTENSION_PBM = 7,
			IMAGE_EXTENSION_PBMRAW = 8,
			IMAGE_EXTENSION_PCD = 9,
			IMAGE_EXTENSION_PCX = 10,
			IMAGE_EXTENSION_PGM = 11,
			IMAGE_EXTENSION_PGMRAW = 12,
			IMAGE_EXTENSION_PNG = 13,
			IMAGE_EXTENSION_PPM = 14,
			IMAGE_EXTENSION_PPMRAW = 15,
			IMAGE_EXTENSION_RAS = 16,
			IMAGE_EXTENSION_TARGA = 17,
			IMAGE_EXTENSION_TIFF = 18,
			IMAGE_EXTENSION_WBMP = 19,
			IMAGE_EXTENSION_PSD = 20,
			IMAGE_EXTENSION_CUT = 21,
			IMAGE_EXTENSION_XBM = 22,
			IMAGE_EXTENSION_XPM = 23,
			IMAGE_EXTENSION_DDS = 24,
			IMAGE_EXTENSION_GIF = 25,
			IMAGE_EXTENSION_HDR = 26,
			IMAGE_EXTENSION_FAXG3 = 27,
			IMAGE_EXTENSION_SGI = 28,
			IMAGE_EXTENSION_EXR = 29,
			IMAGE_EXTENSION_J2K = 30,
			IMAGE_EXTENSION_JP2 = 31,
			IMAGE_EXTENSION_PFM = 32,
			IMAGE_EXTENSION_PICT = 33,
			IMAGE_EXTENSION_RAW = 34,
			IMAGE_EXTENSION_WEBP = 35,
			IMAGE_EXTENSION_JXR = 36
		};


		struct AssetLoadingDesc
		{
			AssetLoadingDesc()
				: mAsyncLoading(true) , pMetadata(nullptr)
			{

			}
			AssetMetadata* pMetadata;
			bool mAsyncLoading = true;
		};

		struct TextureLoadingDesc : public AssetLoadingDesc
		{
			TextureLoadingDesc()
				: mExtension(IMAGE_EXTENSION_UNKNOWN)
			{

			}

			IMAGE_EXTENSION mExtension;

			Utilities::DataBlob mData;
		};

	
	}
}