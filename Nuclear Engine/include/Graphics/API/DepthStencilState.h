#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLDepthStencilState.h>
#include <Graphics/API/DirectX\DX11DepthStencilState.h>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			class NEAPI DepthStencilState : public CBaseAPI<OpenGL::GLDepthStencilState, DirectX::DX11DepthStencilState>
			{
			public:
				DepthStencilState();
				~DepthStencilState();

				static void Create(DepthStencilState* state, const DepthStencilStateDesc& type);
				static void Delete(DepthStencilState* state);

				void Bind();
			};
		}
	}
}