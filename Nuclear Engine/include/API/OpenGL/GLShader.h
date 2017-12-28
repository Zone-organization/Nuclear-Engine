#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
namespace NuclearEngine
{
	struct BinaryShaderBlob;
	enum class ShaderType;
	class GLConstantBuffer;
	namespace API
	{
		namespace OpenGL
		{
			class NEAPI GLShader
			{
			public:
				GLShader();
				~GLShader();

				static GLShader Create(
					NuclearEngine::BinaryShaderBlob* VertexShaderCode,
					NuclearEngine::BinaryShaderBlob* PixelShaderCode,
					NuclearEngine::BinaryShaderBlob* GeometryShaderCode);
				
				void SetConstantBuffer(GLConstantBuffer* ubuffer, NuclearEngine::ShaderType type);
				
				void Bind();
				void Unbind();

				//OpenGL Specific
				unsigned int GetGLShaderID();

				//DirectX 11 Specific  Note: Do Nothing here!!
				void * GetDXBufferPointer();
				unsigned long GetDXBufferSize();

			private:
				GLuint _ProgramID;
			};
		}
	}
}
#endif