#pragma once

/*------------------------PROTOTYPE ENGINE- SHADER------------------------------------
--Loads Shaders (GLSL , HLSL)
--------------------------------------------------------------------------------------
---Usage:
--Include This Header to Use Shader Loader
--------------------------------------------------------------------------------------
---Status: WORKING
--------------------------------------------------------------------------------------
---Notes

HLSL Versions:
--------------
VS_PS_4_0   = Shader Model 4.0, Radeon HD 2000 and GeForce 8, DirectX 10 

Used by:
Directx 11
------------------------------------ZLIXINE-------------------------------------------
*/

#include <NE_Common.h>
#include <API\UniformBuffer.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBShader;
}

namespace NuclearEngine {
	namespace API {

		class NEAPI Shader
		{
		public:
			Shader(const char *name, const char* VertexShader, const char* PixelShader, const char* GeometryShader, ShaderType inputtype);
			~Shader();

			void SetUniformBuffer(UniformBuffer* cbuffer);
			void Bind();
			void Unbind();

			NuclearRenderer::NRBShader* GetBase();
		protected:
			NuclearRenderer::NRBShader *shader;
		};
	}
}