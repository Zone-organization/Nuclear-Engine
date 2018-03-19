#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics/API/OpenGL\GLBlendState.h>
#include <Graphics/API/DirectX\DX11BlendState.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			class NEAPI BlendState
			{
			public:
				BlendState();
				~BlendState();

				static void Create(BlendState* state, const BlendStateDesc& type);
				static void Delete(BlendState* state);

				void Bind(Graphics::Color blendfactor = Graphics::Color(0.0f), unsigned int samplemask = 0xffffffff);
			private:
				OpenGL::GLBlendState GLObject;
				DirectX::DX11BlendState DXObject;
			};
		}
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLBlendState.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{

			typedef OpenGL::GLBlendState BlendState;
		}
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <Graphics/API/DirectX\DX11BlendState.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef DirectX::DX11BlendState BlendState;
		}
	}
}
#endif
#endif