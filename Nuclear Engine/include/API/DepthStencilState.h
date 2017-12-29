#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDERER
#include <API\OpenGL\GLDepthStencilState.h>
#include <API\DirectX\DX11DepthStencilState.h>

namespace NuclearEngine {
	namespace API {
		class DepthStencilState
		{
		public:
			DepthStencilState();
			~DepthStencilState();

			static void Create(DepthStencilState* state, DepthStencilStateDesc* type);

			void Bind();

			void Unbind();
		private:
			OpenGL::GLDepthStencilState GLObject;
			DirectX::DX11DepthStencilState DXObject;
		};
	}
}
#else
#ifdef NE_USE_OPENGL3_3
#include <API\OpenGL\GLDepthStencilState.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLDepthStencilState DepthStencilState;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11DepthStencilState.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11DepthStencilState DepthStencilState;
	}
}
#endif
#endif