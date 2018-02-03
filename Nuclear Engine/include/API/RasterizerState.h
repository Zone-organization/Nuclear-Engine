#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLRasterizerState.h>
#include <API\DirectX\DX11RasterizerState.h>

namespace NuclearEngine {
	namespace API {
		class NEAPI RasterizerState
		{
		public:
			RasterizerState();
			~RasterizerState();

			static void Create(RasterizerState* state, const RasterizerStateDesc& type);
			static void Delete(RasterizerState* state);

			void Bind();

			static void Bind_Default();
		private:
			OpenGL::GLRasterizerState GLObject;
			DirectX::DX11RasterizerState DXObject;
		};
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLRasterizerState.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLRasterizerState RasterizerState;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11RasterizerState.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11RasterizerState RasterizerState;
	}
}
#endif
#endif