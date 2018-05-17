#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/Color.h>

namespace NuclearEngine
{
	namespace Graphics
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
			};

			namespace OpenGL
			{
				class NEAPI GLBlendState
				{
				public:
					GLBlendState();
					~GLBlendState();

					static void Create(GLBlendState *state, const BlendStateDesc& type);
					static void Delete(GLBlendState *state);

					void Bind(Graphics::Color blendfactor = Graphics::Color(0.0f), unsigned int samplemask = 0xffffffff);
				protected:
					bool IndependentBlendEnable;
					OpenGL_RT_Blend targets[8];
				};
			}
		}
	}
}
#endif