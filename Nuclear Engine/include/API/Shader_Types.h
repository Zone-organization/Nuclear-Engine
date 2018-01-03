#pragma once
#include <string>
#include <BuildConfig.h>

#ifdef NE_COMPILE_D3DCOMPILER
#include <d3dcompiler.h>
#endif

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


		struct BinaryShaderBlob 
		{			
#ifdef NE_COMPILE_D3DCOMPILER
			ID3D10Blob* DXBC_SourceCode;
#endif

#ifdef NE_COMPILE_OPENGL3_3
			std::string GLSL_SourceCode;
#endif
			ShaderLanguage Language;
		};

	}
}