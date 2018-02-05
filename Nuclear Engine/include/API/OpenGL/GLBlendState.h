#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace API
	{
		struct BlendStateDesc;
		namespace OpenGL
		{
			class NEAPI GLBlendState
			{
			public:
				GLBlendState();
				~GLBlendState();

				static void Create(GLBlendState *state, const BlendStateDesc& type);
				static void Delete(GLBlendState *state);

				void Bind();

				static void Bind_Default();
			protected:
			
			};
		}
	}
}
#endif