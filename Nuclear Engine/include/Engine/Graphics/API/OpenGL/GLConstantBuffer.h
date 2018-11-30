#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				class NEAPI GLConstantBuffer
				{
				public:
					GLConstantBuffer();
					~GLConstantBuffer();

					static void Create(GLConstantBuffer* buffer, const char *Nameinshader, unsigned int size);
					static void Delete(GLConstantBuffer* cbuffer);

					void Update(const void* data, unsigned int size);

					void VSBind(Uint8 slot);
					void GSBind(Uint8 slot);
					void PSBind(Uint8 slot);

					GLuint mCBuffer;
					GLuint mBindingIndex;
					const char* mName;

				};
			}
		}
	}
}

#endif
