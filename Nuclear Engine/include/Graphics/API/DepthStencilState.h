#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics/API/OpenGL\GLDepthStencilState.h>
#include <Graphics/API/DirectX\DX11DepthStencilState.h>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			class NEAPI DepthStencilState
			{
			public:
				DepthStencilState();
				~DepthStencilState();

				static void Create(DepthStencilState* state, const DepthStencilStateDesc& type);
				static void Delete(DepthStencilState* state);

				void Bind();
			private:
				OpenGL::GLDepthStencilState GLObject;
				DirectX::DX11DepthStencilState DXObject;
			};
		}
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLDepthStencilState.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef OpenGL::GLDepthStencilState DepthStencilState;
		}
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <Graphics/API/DirectX\DX11DepthStencilState.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef DirectX::DX11DepthStencilState DepthStencilState;
		}
	}
}
#endif
#endif