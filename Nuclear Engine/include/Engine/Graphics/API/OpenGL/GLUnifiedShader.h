#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct BinaryShaderBlob;
			namespace OpenGL
			{
				class GLConstantBuffer;
				class GLTexture;
				class NEAPI GLUnifiedShader
				{
					friend class GLTexture;
				public:
					GLUnifiedShader();
					~GLUnifiedShader();

					static void	Create(GLUnifiedShader* shader, BinaryShaderBlob* vshader, BinaryShaderBlob* pshader);
					static void Delete(GLUnifiedShader* shader);

					void SetConstantBuffer(GLConstantBuffer* ubuffer);

					void Bind();

					//OpenGL only!
					static GLuint GetID();
				private:
					GLuint _ProgramID;
					static void Fix_Reflected_ConstantBuffer_Slot(GLUnifiedShader* result, BinaryShaderBlob* blob);
				};
			}
		}
	}
}
#endif
