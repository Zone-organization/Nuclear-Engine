#include <Assets\Importers\TextureImporter.h>
#include <FreeImage\Source\FreeImage.h>
#include <Diligent/Common/interface/Align.hpp>
#include <Assets\Texture.h>
#include <Graphics/GraphicsModule.h>
#include <Graphics/GraphicsModule.h>

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
			TextureImporter::TextureImporter()
			{
				FreeImage_Initialise();
				FreeImage_SetOutputMessage(MyMessageFunc);
			}

			bool TextureImporter::FreeimageLoadMemory(IMAGE_EXTENSION format, Assets::TextureDesc* result, const Assets::TextureImportingDesc& Desc)
			{
				FIBITMAP* dib = nullptr;

				FIMEMORY* memBuff;
				memBuff = FreeImage_OpenMemory((Byte*)Desc.mMemData, Desc.mMemSize);

				FREE_IMAGE_FORMAT imagefmt = (FREE_IMAGE_FORMAT)format;

				if (imagefmt == FIF_UNKNOWN)
				{
					imagefmt = FreeImage_GetFileTypeFromMemory(memBuff, Desc.mMemSize);
				}

				dib = FreeImage_LoadFromMemory(imagefmt, memBuff);
				FreeImage_CloseMemory(memBuff);

				if (!dib)
				{
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
				else if (type == FIT_UINT16)
				{
					bitmap = FreeImage_ConvertToRGB16(dib);
					type = FreeImage_GetImageType(bitmap);
				}

				if (type == FIT_RGBF)
				{
					bitmap = FreeImage_ConvertToRGBAF(dib);
					type = FreeImage_GetImageType(bitmap);
				}

				result->mType = Diligent::RESOURCE_DIM_TEX_2D;
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
					result->mComponentType = Diligent::VT_UINT8;
					break;
				case FIT_UINT16:
				case FIT_RGB16:
				case FIT_RGBA16:
					samples = bytespp / sizeof(WORD);
					result->mComponentType = Diligent::VT_UINT16;
					break;
				case FIT_FLOAT:
				case FIT_RGBF:
				case FIT_RGBAF:
					samples = bytespp / sizeof(float);
					result->mComponentType = Diligent::VT_FLOAT32;
					break;
				}

				result->mNumComponents = samples;

				FreeImage_Unload(dib);

				//result->mRowStride = AlignUp(static_cast<Uint32>(result->mWidth * result->mNumComponents), 4u);

				return true;
			}

			TextureImporter::~TextureImporter()
			{
				FreeImage_DeInitialise();
			}

			bool TextureImporter::Load(const std::string& Path, Assets::TextureDesc* result, const Assets::TextureImportingDesc& Desc)
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
					//Log.Error("[TextureImporter] Failed To Load: " + Path +  ".\n");
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
				result->mType = Diligent::RESOURCE_DIM_TEX_2D;
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
					result->mComponentType = Diligent::VT_UINT8;
					break;
				case FIT_UINT16:
				case FIT_RGB16:
				case FIT_RGBA16:
					samples = bytespp / sizeof(WORD);
					result->mComponentType = Diligent::VT_UINT16;
					break;
				case FIT_FLOAT:
				case FIT_RGBF:
				case FIT_RGBAF:
					samples = bytespp / sizeof(float);
					result->mComponentType = Diligent::VT_FLOAT32;
					break;
				}

				result->mNumComponents = samples;

				FreeImage_Unload(dib);

				//result->mRowStride = AlignUp(static_cast<Uint32>(result->mWidth * result->mNumComponents), 4u);

				return true;
			}

			bool TextureImporter::IsExtensionSupported(const std::string& extension)
			{

				FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

				fif = FreeImage_GetFIFFromFilename(extension.c_str());

				if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
				{

					return true;
				}

				return false;
			}
			TextureImporter& TextureImporter::Get()
			{
				static TextureImporter instance;
				return instance;
			}
			bool TextureImporter::Import(TextureData* data, IMAGE_EXTENSION extension, const Assets::TextureImportingDesc& importingdesc)
			{
				FREE_IMAGE_FORMAT type = (FREE_IMAGE_FORMAT)extension;
				
				if (type == FIF_DDS)
				{
					Diligent::TextureLoadInfo info;
					Diligent::RefCntAutoPtr<Diligent::ITextureLoader> loader;

					Diligent::CreateTextureLoaderFromMemory(importingdesc.mMemData, importingdesc.mMemSize, Diligent::IMAGE_FILE_FORMAT_DDS, false, info, &loader);

					data->mTexDesc = loader->GetTextureDesc();
					data->mSubresources = std::move(loader->GetSubresources());
					data->mMips = std::move(loader->GetMips());
					return true;
				}
				else
				{
					Assets::TextureDesc imagedesc;
					if(FreeimageLoadMemory((IMAGE_EXTENSION)type, &imagedesc, importingdesc))
					{
						Graphics::GraphicsModule::Get().CreateImageData(data, imagedesc);
						return true;
					}
					return false;
				}				
								
				return false;
			}
			bool TextureImporter::Export(const std::string& exportPath, TextureData* data, IMAGE_EXTENSION type)
			{
				if (!data)
				{
					return false;
				}

				if (type == IMAGE_EXTENSION_DDS)
				{
					Diligent::TextureData texdata{ data->mSubresources.data(), static_cast<Uint32>(data->mSubresources.size()) };
					return SaveTextureAsDDS(exportPath.c_str(), data->mTexDesc, texdata);
				}
				
				//TODO:
				// FREEIMAGE EXPORTING
				// 
				//FreeImage_FIFSupportsExportType((FREE_IMAGE_FORMAT)desc.mExtension)
				return false;
			}
			bool TextureImporter::Load(const Assets::TextureLoadingDesc& Desc, TextureData* result)
			{
				if (Desc.mExtension == IMAGE_EXTENSION_DDS)
				{
					Diligent::TextureLoadInfo info;
					Diligent::ITextureLoader* loader;

					Diligent::CreateTextureLoaderFromMemory(result->mData.GetBuffer().data(), result->mData.GetBuffer().size(), Diligent::IMAGE_FILE_FORMAT_DDS,false, info, &loader);

					result->mTexDesc = loader->GetTextureDesc();
					result->mSubresources = std::move(loader->GetSubresources());
					result->mMips = std::move(loader->GetMips());
					return true;
				}
				else
				{
					//FreeimageLoadMemory()
				}
				return false;
			}
			IMAGE_EXTENSION TextureImporter::GetImageExtension(const std::string& filename)
			{
				FREE_IMAGE_FORMAT type = FIF_UNKNOWN;

				type = FreeImage_GetFileType(filename.c_str(), 0);

				return (IMAGE_EXTENSION)type;
			}
		}
	}
}