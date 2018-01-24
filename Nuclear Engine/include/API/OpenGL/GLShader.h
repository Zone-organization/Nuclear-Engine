#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
{
	namespace API
	{		
		struct ShaderDesc;
		enum class ShaderType;
		struct BinaryShaderBlob;
		namespace OpenGL
		{
			class GLConstantBuffer;
			class GLTexture;
			class NEAPI GLShader
			{
				friend class GLTexture;
			public:
				GLShader();
				~GLShader();

				static void Create(GLShader* shader, ShaderDesc* Desc);
				static void Delete(GLShader* shader);

				void SetConstantBuffer(GLConstantBuffer* ubuffer, ShaderType type);
				
				void Bind();
			private:
				GLuint _ProgramID;
				static void Fix_Reflected_ConstantBuffer_Slot(GLShader* result, BinaryShaderBlob* blob);
			};
		}
	}
}
#endif