#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLRasterizerState.h>
#include <Engine/Graphics/API/DirectX\DX11RasterizerState.h>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			class NEAPI RasterizerState //<OpenGL::GLRasterizerState, DirectX::DX11RasterizerState>
			{
			public:
				RasterizerState();
				~RasterizerState();

				static void Create(RasterizerState* state, const RasterizerStateDesc& type);
				static void Delete(RasterizerState* state);

				void Bind();
				BASE_API(RasterizerState)

			};
		}
	}
}