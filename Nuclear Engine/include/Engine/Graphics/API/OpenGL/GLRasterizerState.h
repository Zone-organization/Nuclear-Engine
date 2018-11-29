#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct RasterizerStateDesc;
			namespace OpenGL
			{
				class NEAPI GLRasterizerState
				{
				public:
					GLRasterizerState();
					~GLRasterizerState();

					static void Create(GLRasterizerState *state, const RasterizerStateDesc& type);
					static void Delete(GLRasterizerState *state);

					void Bind();

					bool mCullEnabled;
					GLenum mCullFaceMode;
					GLenum mFrontFaceOrientation;
					GLenum mFillMode;
				};
			}
		}
	}
}
#endif