#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

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

					static void Create(GLPixelShader* shader, const BinaryShaderBlob& Desc);
					static void Delete(GLPixelShader* shader);

					void SetConstantBuffer(GLConstantBuffer* ubuffer);

					void Bind();

					GLuint mFragmentShaderID;
				};
			}
		}
	}
}
#endif