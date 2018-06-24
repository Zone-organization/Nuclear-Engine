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

					static bool	Create(GLUnifiedShader* shader, const BinaryShaderBlob& vshader, const BinaryShaderBlob& pshader);
					static void Delete(GLUnifiedShader* shader);

					void SetConstantBuffer(GLConstantBuffer* ubuffer);

					void Bind();

					//OpenGL only!
					GLuint GetID();
				private:
					GLuint ShaderID;
					static void Fix_Reflected_ConstantBuffer_Slot(GLUnifiedShader* result, const BinaryShaderBlob& blob);
				};
			}
		}
	}
}
#endif
