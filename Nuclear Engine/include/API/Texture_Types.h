#pragma once
#include <API\Format.h>
namespace NuclearEngine
{
	namespace API
	{
		struct Texture_Data {
			int Width = 0;
			int Height = 0;
			int Depth = 0;
			int Components_Number = 0;
			unsigned char *Img_Data_Buf = NULL;
		};

		enum class TextureWrap { Repeat, MirroredReapeat, ClampToEdge, ClampToBorder };
		enum class TextureFilter { Point2D, Linear2D, Point, Bilinear, Trilinear };
		enum class TextureType { Texture1D, Texture2D, Texture3D, TextureCube };
		enum class AnisotropicFilter { None, AnisotropicX2, AnisotropicX4, AnisotropicX8, AnisotropicX16 };

		struct Texture_Desc
		{
			TextureType Type = TextureType::Texture2D;
			Format Format = Format::R8G8B8A8;
			TextureWrap Wrap = TextureWrap::ClampToEdge;
			TextureFilter Filter = TextureFilter::Point;
			AnisotropicFilter AnisoFilter = AnisotropicFilter::None;
		};
	}
}