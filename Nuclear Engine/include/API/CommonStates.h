#pragma once
#include <API\DepthStencilState.h>
#include <API\RasterizerState.h>
#include <API\BlendState.h>

namespace NuclearEngine
{
	namespace API 
	{
		//Common States
		static DepthStencilState DisabledDepthStencil;
		static DepthStencilState EnabledDepthStencil;
		static DepthStencilState DisabledDepth_EnabledStencil;
		static DepthStencilState EnabledDepth_DisabledStencil;

		static DepthStencilState DefaultDepthStencil;

		static RasterizerState DefaultRasterizer;
		static BlendState DefaultBlendState;

	}
}
