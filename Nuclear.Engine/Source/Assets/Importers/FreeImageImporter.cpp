#include <Assets\Importers\FreeImageImporter.h>
#include <FreeImage\Source\FreeImage.h>
#include <Diligent/Common/interface/Align.hpp>
#include <Assets\Image.h>
#include <Graphics\Context.h>

#include "..\..\ThirdParty\DiligentTextureLoader\src\TextureLoader.h"

#pragma comment(lib,"Freeimage.lib")

namespace Nuclear
{
	namespace Assets
	{
		namespace Importers 
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
			void MyMessageFunc(FREE_IMAGE_FORMAT fif, const char* message)
			{
				std::cout << "\n*** " << message << " ***\n";
			}
			FreeImageImporter::FreeImageImporter()
			{
				FreeImage_Initialise();
				FreeImage_SetOutputMessage(MyMessageFunc);
			}

			bool FreeImageImporter::FreeimageLoad(const std::string& Path, Assets::ImageData* result, const Assets::ImageImportingDesc& Desc)
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
					return false;
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

				result->mData = FreeImage_GetBits(bitmap);
				result->mWidth = FreeImage_GetWidth(bitmap);
				result->mHeight = FreeImage_GetHeight(bitmap);
				result->mBitsPerPixel = FreeImage_GetBPP(bitmap);

				unsigned bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
				unsigned samples = 4;

				switch (type)
				{
				case FIT_BITMAP:
					samples = bytespp / sizeof(BYTE);
					result->mComponentType = VT_UINT8;
					break;
				case FIT_UINT16:
				case FIT_RGB16:
				case FIT_RGBA16:
					samples = bytespp / sizeof(WORD);
					result->mComponentType = VT_UINT16;
					break;
				case FIT_FLOAT:
				case FIT_RGBF:
				case FIT_RGBAF:
					samples = bytespp / sizeof(float);
					result->mComponentType = VT_FLOAT32;
					break;
				}

				result->mNumComponents = samples;

				FreeImage_Unload(dib);

				//result->mRowStride = AlignUp(static_cast<Uint32>(result->mWidth * result->mNumComponents), 4u);

				return true;
			}

			FreeImageImporter::~FreeImageImporter()
			{
				FreeImage_DeInitialise();
			}

			bool FreeImageImporter::Load(const std::string& Path, Assets::ImageData* result, const Assets::ImageImportingDesc& Desc)
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
					return false;
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

				result->mData = FreeImage_GetBits(bitmap);
				result->mWidth = FreeImage_GetWidth(bitmap);
				result->mHeight = FreeImage_GetHeight(bitmap);
				result->mBitsPerPixel = FreeImage_GetBPP(bitmap);

				unsigned bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
				unsigned samples = 4;

				switch (type)
				{
				case FIT_BITMAP:
					samples = bytespp / sizeof(BYTE);
					result->mComponentType = VT_UINT8;
					break;
				case FIT_UINT16:
				case FIT_RGB16:
				case FIT_RGBA16:
					samples = bytespp / sizeof(WORD);
					result->mComponentType = VT_UINT16;
					break;
				case FIT_FLOAT:
				case FIT_RGBF:
				case FIT_RGBAF:
					samples = bytespp / sizeof(float);
					result->mComponentType = VT_FLOAT32;
					break;
				}

				result->mNumComponents = samples;

				FreeImage_Unload(dib);

				//result->mRowStride = AlignUp(static_cast<Uint32>(result->mWidth * result->mNumComponents), 4u);

				return true;
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
			bool FreeImageImporter::Import(const std::string& importPath, const std::string& exportPath, Assets::Image* image, const Assets::ImageImportingDesc& desc)
			{
				Assets::ImageData data;
				Assets::ImageCreationDesc creationdesc(data, desc);
				creationdesc.mDeleteDataAfterCreation = false;

				if (!FreeimageLoad(importPath, &data, desc))
				{
					return false;
				}

				Diligent::TextureDesc TexDesc;
				Diligent::TextureData TexData;

				image->Create(creationdesc, TexDesc, TexData);
								
				return SaveTextureAsDDS(exportPath.c_str(), TexDesc, TexData);				
			}
			bool FreeImageImporter::Load(Assets::Image* result, const Assets::ImageLoadingDesc& Desc)
			{
				FREE_IMAGE_FORMAT type = FIF_UNKNOWN;
				if(Desc.mPath != "")
				{
					type = FreeImage_GetFileType(Desc.mPath.c_str(), 0);
				}
				else
				{
					
				}

				if (type == FIF_DDS)
				{
					TextureLoadInfo info;
					RefCntAutoPtr<ITextureLoader> loader;
					RefCntAutoPtr<ITexture> texture;

					CreateTextureLoaderFromMemory(Desc.mData.GetBuffer().data(), Desc.mData.GetBuffer().size(), IMAGE_FILE_FORMAT_DDS,false, info, &loader);

					loader->CreateTexture(Graphics::Context::GetInstance().GetDevice(), &texture);
					result->SetTextureView(texture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
					return true;
				}
				return false;
			}
		}
	}
}