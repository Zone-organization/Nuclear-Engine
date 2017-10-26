#pragma once

//To be used with a client
namespace NuclearEngine {

	enum class ShaderType {
		HLSL,
		DXBC,
		GLSL
	};

	enum class TextureFormat { R8 = 8, R8G8 = 16, R8G8B8 = 24, R8G8B8A8 = 32 };
	enum class TextureWrap { Repeat, MirroredReapeat, ClampToEdge, ClampToBorder };
	enum class TextureFilter { Point2D, Linear2D, Point, Bilinear, Trilinear };
	enum class AnisotropicFilter { None, AnisotropicX2, AnisotropicX4, AnisotropicX8, AnisotropicX16 };

	struct Texture_Desc
	{
		TextureFormat Format = TextureFormat::R8G8B8A8;
		TextureWrap Wrap = TextureWrap::ClampToEdge;
		TextureFilter Filter = TextureFilter::Point;
		AnisotropicFilter AnisoFilter = AnisotropicFilter::None;
	};

	struct Texture_Data {
		int width;
		int height;
		int no_of_components;
		unsigned char *databuf;
	};

}