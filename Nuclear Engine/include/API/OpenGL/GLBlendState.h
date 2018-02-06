#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace API
	{
		struct BlendStateDesc;

		struct OpenGL_RT_Blend
		{
			bool BlendEnable;
			GLenum SrcBlend;
			GLenum DestBlend;
			GLenum BlendOp;
			GLenum SrcBlendAlpha;
			GLenum DestBlendAlpha;
			GLenum BlendOpAlpha;
			unsigned char RenderTargetWriteMask;
		} ;

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
				bool IndependentBlendEnable;
				OpenGL_RT_Blend targets[8];
			};
		}
	}
}
#endif