#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics/API/OpenGL\GLRasterizerState.h>
#include <Graphics/API/DirectX\DX11RasterizerState.h>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			class NEAPI RasterizerState
			{
			public:
				RasterizerState();
				~RasterizerState();

				static void Create(RasterizerState* state, const RasterizerStateDesc& type);
				static void Delete(RasterizerState* state);

				void Bind();
			private:
				OpenGL::GLRasterizerState GLObject;
				DirectX::DX11RasterizerState DXObject;
			};
		}
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLRasterizerState.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef OpenGL::GLRasterizerState RasterizerState;
		}
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <Graphics/API/DirectX\DX11RasterizerState.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef DirectX::DX11RasterizerState RasterizerState;
		}
	}
}
#endif
#endif