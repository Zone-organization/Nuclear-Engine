#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLBlendState.h>
#include <Graphics/API/DirectX\DX11BlendState.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			class NEAPI BlendState : public CBaseAPI<OpenGL::GLBlendState, DirectX::DX11BlendState>
			{
			public:
				BlendState();
				~BlendState();

				static void Create(BlendState* state, const BlendStateDesc& type);
				static void Delete(BlendState* state);

				void Bind(Graphics::Color blendfactor = Graphics::Color(0.0f), unsigned int samplemask = 0xffffffff);
			};
		}
	}
}