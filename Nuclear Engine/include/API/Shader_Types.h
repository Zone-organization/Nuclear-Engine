#pragma once
#include <string>
#include <BuildConfig.h>
#include <map>
#include <API\Texture_Types.h>

namespace NuclearEngine 
{
	namespace API 
	{
		enum class ShaderLanguage {
			HLSL,
			DXBC,
			GLSL
		};

		enum class ShaderType {
			Vertex,
			Pixel,
			Geometry,

			Vertex_Pixel,
			Vertex_Geometry,
			Vertex_Pixel_Geometry
		};


		struct Reflected_Constantbuffer
		{
			unsigned int BindingSlot = 0;
			unsigned int Size = 0;
		};
		struct Reflected_Texture
		{
			unsigned int BindingSlot = 0;
			TextureType Type;
		};

		struct ShaderReflection
		{
			std::map<std::string, Reflected_Constantbuffer> ConstantBuffers;
			std::map<std::string, Reflected_Texture> Textures;
		};

#ifdef NE_COMPILE_D3DCOMPILER
		struct DXBC_BLOB
		{
			void* Buffer;
			size_t Size;
		};
#endif

		struct BinaryShaderBlob 
		{			
#ifdef NE_COMPILE_D3DCOMPILER
			DXBC_BLOB DXBC_SourceCode = DXBC_BLOB();
#endif
#ifdef NE_COMPILE_CORE_OPENGL
			std::string GLSL_SourceCode;
#endif
			ShaderReflection Reflection;
			ShaderLanguage Language;

			//If this blob is crosscompiled (as HLSL 2 GLSL)
			bool Converted = false;

			//A usable shader??
			bool Finished = false;
		};

		struct ShaderDesc
		{
			std::string Name = "NoString";
			BinaryShaderBlob VertexShaderCode = BinaryShaderBlob();
			BinaryShaderBlob PixelShaderCode = BinaryShaderBlob();
			BinaryShaderBlob GeometryShaderCode = BinaryShaderBlob();
		};

	}
}