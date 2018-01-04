#pragma once
#include <string>
#include <BuildConfig.h>

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
			DXBC_BLOB DXBC_SourceCode;
#endif
#ifdef NE_COMPILE_OPENGL3_3
			std::string GLSL_SourceCode;
#endif
			ShaderLanguage Language;
		};

		struct ShaderReflection {

		};

	}
}