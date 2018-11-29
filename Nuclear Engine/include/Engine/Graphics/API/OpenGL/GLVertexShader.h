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
				class NEAPI GLVertexShader
				{
					friend class GLTexture;
				public:
					GLVertexShader();
					~GLVertexShader();

					static void Create(GLVertexShader* shader, const BinaryShaderBlob& Desc);
					static void Delete(GLVertexShader* shader);

					void SetConstantBuffer(GLConstantBuffer* ubuffer);

					void Bind();

					//OpenGL only!
					static GLuint GetPipelineID();

					GLuint mVertexShaderID;
				};
			}
		}
	}
}
#endif