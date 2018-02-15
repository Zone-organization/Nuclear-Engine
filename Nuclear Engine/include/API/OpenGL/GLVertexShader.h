#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
{
	namespace API
	{
		struct BinaryShaderBlob;
		namespace OpenGL
		{
			class GLConstantBuffer;
			class GLTexture;
			class NEAPI GLVertexShader
			{
				friend class GLTexture;
			public:
				GLVertexShader();
				~GLVertexShader();

				static void Create(GLVertexShader* shader, BinaryShaderBlob* Desc);
				static void Delete(GLVertexShader* shader);

				void SetConstantBuffer(GLConstantBuffer* ubuffer);

				void Bind();

				//OpenGL only!
				static GLuint GetPipelineID();
			private:
				GLuint _ProgramID;
				static void Fix_Reflected_ConstantBuffer_Slot(GLVertexShader* result, BinaryShaderBlob* blob);
			};
		}
	}
}
#endif