#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLDepthStencilState.h>
#include <Engine/Graphics/API/DirectX\DX11DepthStencilState.h>

namespace NuclearEngine 
{
	namespace Graphics
	{
		namespace API 
		{
			class NEAPI DepthStencilState //<OpenGL::GLDepthStencilState, DirectX::DX11DepthStencilState>
			{
			public:
				DepthStencilState();
				~DepthStencilState();

				static void Create(DepthStencilState* state, const DepthStencilStateDesc& type);
				static void Delete(DepthStencilState* state);

				void Bind();
				BASE_API(DepthStencilState)
			};
		}
	}
}