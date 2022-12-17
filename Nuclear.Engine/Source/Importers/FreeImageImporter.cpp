#include <Importers\Internal\FreeImageImporter.h>
#include <FreeImage\Source\FreeImage.h>
#include <Diligent/Common/interface/Align.hpp>

#pragma comment(lib,"Freeimage.lib")

namespace Nuclear
{
	namespace Importers
	{
		namespace Internal 
		{
			/// INPLACESWAP adopted from codeguru.com 
			template <class T> void INPLACESWAP(T& a, T& b) {
				a ^= b; b ^= a; a ^= b;
			}

			BOOL SwapRedBlue32(FIBITMAP* dib)
			{
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

			FreeImageImporter::FreeImageImporter()
			{
				FreeImage_Initialise();
			}

			FreeImageImporter::~FreeImageImporter()
			{
				FreeImage_DeInitialise();
			}

			Assets::ImageData FreeImageImporter::Load(const std::string& Path, const Assets::ImageLoadingDesc& Desc)
			{
				FIBITMAP* dib = nullptr;

				if (Desc.mLoadFromMemory == true)
				{
					FIMEMORY* memBuff;
					memBuff = FreeImage_OpenMemory((Byte*)Desc.mMemData, Desc.mMemSize);

					auto type = FreeImage_GetFileTypeFromMemory(memBuff, 0);
					dib = FreeImage_LoadFromMemory(type, memBuff);
					FreeImage_CloseMemory(memBuff);
				}
				else
				{
					dib = FreeImage_Load(FreeImage_GetFileType(Path.c_str(), 0), Path.c_str());
				}

				if (!dib)
				{
					//Log.Error("[FreeImageImporter] Failed To Load: " + Path +  ".\n");
					return Assets::ImageData();
				}
				if (Desc.mFlipY_Axis)
					FreeImage_FlipVertical(dib);

				FIBITMAP* bitmap = nullptr;
				auto type = FreeImage_GetImageType(dib);

				//Only bitmaps are loaded in BGR format in sume platforms
				//TODO: Disable if its already RGB
				if (type == FIT_BITMAP)
				{
					bitmap = FreeImage_ConvertTo32Bits(dib);
					SwapRedBlue32(bitmap);
				}

				if (type == FIT_RGBF)
				{
					bitmap = FreeImage_ConvertToRGBAF(dib);
					type = FreeImage_GetImageType(bitmap);
				}

				Assets::ImageData result;
				result.mData = FreeImage_GetBits(bitmap);
				result.mWidth = FreeImage_GetWidth(bitmap);
				result.mHeight = FreeImage_GetHeight(bitmap);
				result.mBitsPerPixel = FreeImage_GetBPP(bitmap);

				unsigned bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
				unsigned samples = 4;

				switch (type)
				{
				case FIT_BITMAP:
					samples = bytespp / sizeof(BYTE);
					result.mComponentType = VT_UINT8;
					break;
				case FIT_UINT16:
				case FIT_RGB16:
				case FIT_RGBA16:
					samples = bytespp / sizeof(WORD);
					result.mComponentType = VT_UINT16;
					break;
				case FIT_FLOAT:
				case FIT_RGBF:
				case FIT_RGBAF:
					samples = bytespp / sizeof(float);
					result.mComponentType = VT_FLOAT32;
					break;
				}

				result.mNumComponents = samples;

				FreeImage_Unload(dib);

				//result.mRowStride = AlignUp(static_cast<Uint32>(result.mWidth * result.mNumComponents), 4u);

				return result;
			}

			bool FreeImageImporter::IsExtensionSupported(const std::string& extension)
			{

				FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

				fif = FreeImage_GetFIFFromFilename(extension.c_str());

				if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
				{

					return true;
				}

				return false;
			}
			FreeImageImporter& FreeImageImporter::GetInstance()
			{
				static FreeImageImporter instance;
				return instance;
			}
		}
	}
}