#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
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

				static void Create(GLConstantBuffer* buffer,const char *Nameinshader, unsigned int size);
				void Update(const void* data, unsigned int size);
				unsigned int GetBindingIndex();
				const char * GetName();

			protected:
				GLuint buffer;
				GLuint BindingIndex;
				const char* name;
				
			};
		}
	}
}

#endif
