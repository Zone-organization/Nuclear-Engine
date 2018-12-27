#define EXPOSE_STB_IMAGE_IMPORTER
#include <Engine\Importers\StbImageImporter.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\ThirdParty\stb_image.h"
#include <Diligent/Common/interface/Align.h>

namespace NuclearEngine 
{
	namespace Importers
	{
		Assets::Image & StbImageLoad(const std::string & Path, const Managers::TextureLoadingDesc & Desc)
		{
			int ReqNumComponents = 0, Channels = 0;
			switch (Desc.mFormat)
			{
			case TEX_FORMAT_R8_UNORM:
				ReqNumComponents = 1;
				Channels = 8;
				break;
			case TEX_FORMAT_RG8_UNORM:
				ReqNumComponents = 2;
				Channels = 8;
				break;
			case TEX_FORMAT_RGBA8_UNORM:
			case TEX_FORMAT_RGBA8_UNORM_SRGB:
				ReqNumComponents = 4;
				Channels = 8;
				break;
			}

			int texWidth = 0, texHeight = 0, texComponents = 0;

			stbi_set_flip_vertically_on_load(Desc.mFlipY_Axis);
			Assets::Image image;

			image.mData = stbi_load(Path.c_str(), &texWidth, &texHeight, &texComponents, ReqNumComponents);
			image.mWidth = texWidth;
			image.mHeight = texHeight;
			image.mBitsPerPixel = Channels * texComponents;
			image.mNumComponents = texComponents;
			image.mRowStride = Align(static_cast<Uint32>(texWidth * texComponents), 4u);

			return image;
		}
	}
}