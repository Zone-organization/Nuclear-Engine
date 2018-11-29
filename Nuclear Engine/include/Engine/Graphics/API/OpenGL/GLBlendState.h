#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/API/RenderStates_Types.h>

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

					bool IndependentBlendEnable;
					OpenGL_RT_Blend targets[8];

					//Helper Functions
					static GLenum BLEND_TO_GLenum(BLEND blend);
					static GLenum BLEND_OP_TO_GLenum(BLEND_OP blend);
					static BLEND GLenum_TO_BLEND(GLenum blend);
					static BLEND_OP GLenum_TO_BLEND_OP(GLenum blend);
				};
			}
		}
	}
}
#endif