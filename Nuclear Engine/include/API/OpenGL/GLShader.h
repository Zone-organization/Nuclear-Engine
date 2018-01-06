#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_OPENGL3_3

namespace NuclearEngine
{
	namespace API
	{		
		struct ShaderDesc;
		enum class ShaderType;
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
				
				void SetConstantBuffer(GLConstantBuffer* ubuffer, ShaderType type);
				
				void Bind();
			private:
				GLuint _ProgramID;
			};
		}
	}
}
#endif