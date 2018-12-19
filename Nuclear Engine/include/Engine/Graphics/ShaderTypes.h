#pragma once
#include <string>
#include <Base\BuildConfig.h>
#include <unordered_map>
#include <LLGL\TextureFlags.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		enum class ShaderVariableType
		{
			Unknown,
			Bool,
			Int1,
			Int2,
			Int3,
			Int4,
			Float1,
			Float2,	//Vector2
			Float3, //Vector3
			Float4, //Vector4
			Matrix2x2,
			Matrix3x3,
			Matrix4x4,
			Struct
		};

		enum class ResourceType
		{
			Undefined,                  //!< Undfined resource type.

			Texture1D,                  //!< 1D texture: `Texture1D` in HLSL, `texture1D` in GLSL (Vulkan only).
			Texture2D,                  //!< 2D texture: `Texture2D` in HLSL, `texture2D` in GLSL (Vulkan only).
			Texture3D,                  //!< 3D texture: `Texture3D` in HLSL, `texture3D` in GLSL (Vulkan only).
			TextureCube,                //!< Cube texture: `TextureCube` in HLSL, `textureCube` in GLSL (Vulkan only).
			Texture1DArray,             //!< 1D array texture: `Texture1DArray` in HLSL, `texture1DArray` in GLSL (Vulkan only).
			Texture2DArray,             //!< 2D array texture: `Texture2DArray` in HLSL, `texture2DArray` in GLSL (Vulkan only).
			TextureCubeArray,           //!< Cube array texture: `TextureCubeArray` in HLSL, `textureCubeArray` in GLSL (Vulkan only).
			Texture2DMS,                //!< 2D multi-sampled texture: `Texture2DMS` in HLSL, `texture2DMS` in GLSL (Vulkan only).
			Texture2DMSArray,           //!< 2D multi-sampled array texture: `Texture2DMSArray` in HLSL, `texture2DMSArray` in GLSL (Vulkan only).

			RWTexture1D,                //!< 1D read-write texture: `RWTexture1D` in HLSL, `image1D` in GLSL.
			RWTexture2D,                //!< 2D read-write texture: `RWTexture2D` in HLSL, `image2D` in GLSL.
			RWTexture3D,                //!< 3D read-write texture: `RWTexture3D` in HLSL, `image3D` in GLSL.
			RWTextureCube,              //!< Cube read-write texture: `RWTextureCube` in HLSL, `imageCube` in GLSL.
			RWTexture1DArray,           //!< 1D array read-write texture: `RWTexture1DArray` in HLSL, `image1DArray` in GLSL.
			RWTexture2DArray,           //!< 2D array read-write texture: `RWTexture2DArray` in HLSL, `image2DArray` in GLSL.
			RWTextureCubeArray,         //!< Cube array read-write texture: `RWTextureCubeArray` in HLSL, `imageCubeArray` in GLSL.
			RWTexture2DMS,              //!< 2D multi-sampled read-write texture: `RWTexture2DMS` in HLSL, `image2DMS` in GLSL.
			RWTexture2DMSArray,         //!< 2D multi-sampled array read-write texture: `RWTexture2DMSArray` in HLSL, `image2DMSArray` in GLSL.

			Sampler1D,                  //!< Combined 1D texture-sampler: `sampler1D` in GLSL.
			Sampler2D,                  //!< Combined 2D texture-sampler: `sampler2D` in GLSL.
			Sampler3D,                  //!< Combined 3D texture-sampler: `sampler3D` in GLSL.
			SamplerCube,                //!< Combined Cube texture-sampler: `samplerCube` in GLSL.
			Sampler1DArray,             //!< Combined 1D array texture-sampler: `sampler1DArray` in GLSL.
			Sampler2DArray,             //!< Combined 2D array texture-sampler: `sampler2DArray` in GLSL.
			SamplerCubeArray,           //!< Combined Cube array texture-sampler: `samplerCubeArray` in GLSL.
			Sampler2DMS,                //!< Combined 2D multi-sampled texture-sampler: `sampler2DMS` in GLSL.
			Sampler2DMSArray,           //!< Combined 2D multi-sampled array texture-sampler: `sampler2DMSArray` in GLSL.
			Sampler2DRect,              //!< Combined 2D texture-sampler with unnormalized texture coordinates: `sampler2DRect` in GLSL.

			Buffer,                     //!< Vector buffer: `Buffer` in HLSL, `samplerBuffer` in GLSL.
			ByteAddressBuffer,          //!< Byte addressable buffer: `ByteAddressBuffer` in HLSL, `samplerBuffer` in GLSL.
			StructuredBuffer,           //!< Structured buffer: `StructuredBuffer` in HLSL, `buffer` in GLSL and VKSL.
			AppendStructuredBuffer,     //!< Append structured buffer: `AppendStructuredBuffer` in HLSL, `buffer` in GLSL.
			ConsumeStructuredBuffer,    //!< Consume structured buffer: `ConsumeStructuredBuffer` in HLSL, `buffer` in GLSL.

			RWBuffer,                   //!< Vector read-write buffer: `RWBuffer` in HLSL, `imageBuffer` in GLSL.
			RWByteAddressBuffer,        //!< Byte addressable read-write buffer: `RWByteAddressBuffer` in HLSL, `imageBuffer` in GLSL.
			RWStructuredBuffer,         //!< Structured read-write buffer: `RWStructuredBuffer` in HLSL, `buffer` in GLSL and VKSL.

			ConstantBuffer,             //!< Constant buffer: `cbuffer` in HLSL, `uniform` in GLSL.
			TextureBuffer,              //!< Texture buffer: `tbuffer` in HLSL, `samplerBuffer` in GLSL.
			SamplerState,               //!< Sampler state: `SamplerState` in HLSL, `sampler` in GLSL (Vulkan only).
			SamplerComparisonState,     //!< Sampler comparison state: `SamplerComparisonState` in HLSL, `sampler` in GLSL (Vulkan only).
		};

		struct ShaderVariable
		{
			Uint32 mSize = 4;           // Size of variable (in bytes)
			ShaderVariableType mType = ShaderVariableType::Float1;
		};

		struct ReflectedShaderResource
		{
			Uint32 mSlot = 0;
			Uint32 mSize = 0;
			ResourceType mType = ResourceType::Undefined;
			std::unordered_map<std::string, ShaderVariable> mVariables;
		};
		struct ShaderReflection
		{
			std::unordered_map<std::string, ReflectedShaderResource> mResources;

		};

		namespace ShaderTypesHelpers
		{
			inline bool IsTexture(const ResourceType& type)
			{
				switch (type)
				{
				case ResourceType::Texture1D:
				case ResourceType::Texture1DArray:
				case ResourceType::Texture2D:
				case ResourceType::Texture2DArray:
				case ResourceType::Texture2DMS:
				case ResourceType::Texture2DMSArray:
				case ResourceType::Texture3D:
				case ResourceType::TextureCube:
				case ResourceType::TextureCubeArray:
					return true;
				}
				return false;
			}

		}

	}
}