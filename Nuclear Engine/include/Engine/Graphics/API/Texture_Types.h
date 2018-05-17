#pragma once
#include <Engine/Graphics/API/Format.h>
#include <Engine/Graphics/Color.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct Texture_Data {
				int Width = 0;
				int Height = 0;
				int Depth = 0;
				int Components_Number = 0;
				bool Valid = false;
				Uint32 HashedName = 0;
				unsigned char *Img_Data_Buf = NULL;
			};

			enum class TextureWrap { Repeat, MirroredRepeat, ClampToEdge, ClampToBorder };
			enum class TextureFilter { Point2D, Linear2D, Point, Bilinear, Trilinear };
			enum class TextureType { Texture1D, Texture2D, Texture3D, TextureCube };
			enum class AnisotropicFilter { None = 0, AnisotropicX2 = 2, AnisotropicX4 = 4, AnisotropicX8 = 8, AnisotropicX16 = 16 };

			struct Texture_Desc
			{
				TextureType Type = TextureType::Texture2D;
				Format Format = Format::R8G8B8A8_UNORM;
				bool GenerateMipMaps = true;
			};

			struct SamplerDesc
			{
				TextureWrap WrapU = TextureWrap::Repeat;
				TextureWrap WrapV = TextureWrap::Repeat;
				TextureWrap WrapW = TextureWrap::Repeat;

				TextureFilter Filter = TextureFilter::Point;

				AnisotropicFilter AnisoFilter = AnisotropicFilter::None;

				Color BorderColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			};
		}
	}
}