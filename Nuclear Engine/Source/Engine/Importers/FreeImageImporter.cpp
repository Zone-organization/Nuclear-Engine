#define EXPOSE_FREEIMAGE_IMPORTER
#include <Engine\Importers\FreeImageImporter.h>
#include <FreeImage/FreeImage.h>
#include <Diligent/Common/interface/Align.h>

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
		Assets::Image& FreeImageLoad(const std::string& Path, const Importers::TextureLoadingDesc& Desc)
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

			Assets::Image image;
			if (!FreeImageInitalized)
			{
				FreeImage_Initialise();
				FreeImageInitalized = true;
			}

			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			FIBITMAP* dib = nullptr;

			fif = FreeImage_GetFileType(Path.c_str(), 0);
			if (fif == FIF_UNKNOWN)
			{
				Log.Error("[FreeImageImporter] Unknown Image type.\n");
			}
			dib = FreeImage_Load(fif, Path.c_str());

			if(Desc.mFlipY_Axis)
				FreeImage_FlipVertical(dib);

			FIBITMAP* bitmap = nullptr;
			bitmap = FreeImage_ConvertTo32Bits(dib);



			if(!SwapRedBlue32(bitmap))
				Log.Error("[FreeImageImporter] SwapRedBlue32 Failed.\n");

			image.mData = FreeImage_GetBits(bitmap);
			image.mWidth = FreeImage_GetWidth(bitmap);
			image.mHeight = FreeImage_GetHeight(bitmap);
			image.mBitsPerPixel = FreeImage_GetBPP(bitmap);

			if (ReqNumComponents == 0)
				image.mNumComponents = image.mBitsPerPixel / Channels;
			else
				image.mNumComponents = ReqNumComponents;

			FreeImage_Unload(dib);

			image.mRowStride = Align(static_cast<Uint32>(image.mWidth * image.mNumComponents), 4u);

			return image;
		}
	}
}