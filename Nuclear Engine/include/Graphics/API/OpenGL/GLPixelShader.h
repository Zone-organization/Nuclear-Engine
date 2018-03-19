#pragma once
#include <Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			enum class ShaderType;
			struct BinaryShaderBlob;
			namespace OpenGL
			{
				class GLConstantBuffer;
				class GLTexture;
				class NEAPI GLPixelShader
				{
					friend class GLTexture;
				public:
					GLPixelShader();
					~GLPixelShader();

					static void Create(GLPixelShader* shader, BinaryShaderBlob* Desc);
					static void Delete(GLPixelShader* shader);

					void SetConstantBuffer(GLConstantBuffer* ubuffer);

					void Bind();
				private:
					GLuint _ProgramID;
					static void Fix_Reflected_ConstantBuffer_Slot(GLPixelShader* result, BinaryShaderBlob* blob);
				};
			}
		}
	}
}
#endif