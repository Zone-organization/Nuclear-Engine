#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
#include <API\OpenGL\GLTexture.h>

namespace NuclearEngine
{
	struct BinaryShaderBlob;
	enum class ShaderType;
	namespace API
	{
		namespace OpenGL
		{
			class GLConstantBuffer;

			class NEAPI GLShader
			{
				friend class GLTexture;
			public:
				GLShader();
				~GLShader();

				static void Create(GLShader* shader,
					NuclearEngine::BinaryShaderBlob* VertexShaderCode,
					NuclearEngine::BinaryShaderBlob* PixelShaderCode,
					NuclearEngine::BinaryShaderBlob* GeometryShaderCode = nullptr);
				
				void SetConstantBuffer(GLConstantBuffer* ubuffer, NuclearEngine::ShaderType type);
				
				void Bind();
			private:
				GLuint _ProgramID;
			};
		}
	}
}
#endif