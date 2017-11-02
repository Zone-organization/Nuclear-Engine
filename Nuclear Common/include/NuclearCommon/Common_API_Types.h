#pragma once

//To be used with a client
namespace NuclearEngine {

	enum class ShaderLanguage {
		HLSL,
		DXBC,
		GLSL
	};

	enum class ShaderType {
		Vertex,
		Pixel,
		Geometry
	};

	enum class TextureFormat { R8 = 8, R8G8 = 16, R8G8B8 = 24, R8G8B8A8 = 32, Depth, Stencil, Depth_Stencil };
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

	enum class BufferGPUUsage {
		Default,
		Static,
		Dynamic
	};

	enum class BufferCPUAccess {
		Default,
		ReadOnly,
		WriteOnly
	};

	enum class Format
	{
		R8,
		R8G8,
		R8G8B8,
		R8G8B8A8,

		R16,
		R16G16,
		R16G16B16,
		R16G16B16A16,

		R32,
		R32G32,
		R32G32B32,
		R32G32B32A32
	};
}