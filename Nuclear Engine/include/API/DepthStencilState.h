#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLDepthStencilState.h>
#include <API\DirectX\DX11DepthStencilState.h>

namespace NuclearEngine {
	namespace API {
		class NEAPI DepthStencilState
		{
		public:
			DepthStencilState();
			~DepthStencilState();

			static void Create(DepthStencilState* state,const DepthStencilStateDesc& type);
			static void Delete(DepthStencilState* state);

			void Bind();

			static void Bind_Default();
		private:
			OpenGL::GLDepthStencilState GLObject;
			DirectX::DX11DepthStencilState DXObject;
		};
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
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