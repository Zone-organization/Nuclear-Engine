#define EXPOSE_FREEIMAGE_IMPORTER
#include <Engine\Importers\FreeImageImporter.h>
#include <FreeImage\Source\FreeImage.h>
#include <Diligent/Common/interface/Align.hpp>

#pragma comment(lib,"Freeimage.lib")

namespace NuclearEngine 
{
	namespace Importers
	{
		/// INPLACESWAP adopted from codeguru.com 
		template <class T> void INPLACESWAP(T& a, T& b) {
			a ^= b; b ^= a; a ^= b;
		}

		BOOL SwapRedBlue32(FIBITMAP* dib) {
			if (FreeImage_GetImageType(dib) != FIT_BITMAP) {
				return FALSE;
			}

			const unsigned bytesperpixel = FreeImage_GetBPP(dib) / 8;
			if (bytesperpixel > 4 || bytesperpixel < 3) {
				return FALSE;
			}

			const unsigned height = FreeImage_GetHeight(dib);
			const unsigned pitch = FreeImage_GetPitch(dib);
			const unsigned lineSize = FreeImage_GetLine(dib);

			BYTE* line = FreeImage_GetBits(dib);
			for (unsigned y = 0; y < height; ++y, line += pitch) {
				for (BYTE* pixel = line; pixel < line + lineSize; pixel += bytesperpixel) {
					INPLACESWAP(pixel[0], pixel[2]);
				}
			}

			return TRUE;
		}

		static bool FreeImageInitalized = false;
		Assets::ImageData FreeImageLoad(const std::string& Path, const Importers::ImageLoadingDesc& Desc)
		{
			int ReqNumComponents = 0, Channels = 8;
			switch (Desc.mFormat)
			{
			case TEX_FORMAT_R8_UNORM:
				ReqNumComponents = 1;
				break;
			case TEX_FORMAT_RG8_UNORM:
				ReqNumComponents = 2;
				break;
			case TEX_FORMAT_RGBA8_UNORM:
			case TEX_FORMAT_RGBA8_UNORM_SRGB:
				ReqNumComponents = 4;
				break;
			}

			if (!FreeImageInitalized)
			{
				FreeImage_Initialise();
				FreeImageInitalized = true;
			}

			FIBITMAP* dib = nullptr;


			if (Desc.mLoadFromMemory == true)
			{
				FIMEMORY* memBuff;
				memBuff = FreeImage_OpenMemory((Byte*)Desc.mMemData, Desc.mMemSize);

				auto type = FreeImage_GetFileTypeFromMemory(memBuff,0);
				dib = FreeImage_LoadFromMemory(type, memBuff);
				FreeImage_CloseMemory(memBuff);
			}
			else
			{
				dib = FreeImage_Load(FreeImage_GetFileType(Path.c_str(), 0), Path.c_str());
			}

			if (!dib)
			{
				Log.Error("[FreeImageImporter] Failed To Load: " + Path +  ".\n");
				return Assets::ImageData();
			}
			if(Desc.mFlipY_Axis)
				FreeImage_FlipVertical(dib);

			FIBITMAP* bitmap = nullptr;
			bitmap = FreeImage_ConvertTo32Bits(dib);



			if (!SwapRedBlue32(bitmap))
			{
				Log.Error("[FreeImageImporter] Failed To Load: " + Path + " , SwapRedBlue32 Failed..\n");
				return Assets::ImageData();
			}
			Assets::ImageData result;
			result.mData = FreeImage_GetBits(bitmap);
			result.mWidth = FreeImage_GetWidth(bitmap);
			result.mHeight = FreeImage_GetHeight(bitmap);
			result.mBitsPerPixel = FreeImage_GetBPP(bitmap);

			if (ReqNumComponents == 0)
				result.mNumComponents = result.mBitsPerPixel / Channels;
			else
				result.mNumComponents = ReqNumComponents;

			FreeImage_Unload(dib);

			result.mRowStride = AlignUp(static_cast<Uint32>(result.mWidth * result.mNumComponents), 4u);

			return result;
		}
	}
}