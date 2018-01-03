#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3
#include <API\OpenGL\GLTexture.h>

namespace NuclearEngine
{
	namespace API
	{
		struct BinaryShaderBlob;
		enum class ShaderType;

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
					BinaryShaderBlob* VertexShaderCode,
					BinaryShaderBlob* PixelShaderCode,
					BinaryShaderBlob* GeometryShaderCode = nullptr);
				
				void SetConstantBuffer(GLConstantBuffer* ubuffer, ShaderType type);
				
				void Bind();
			private:
				GLuint _ProgramID;
			};
		}
	}
}
#endif