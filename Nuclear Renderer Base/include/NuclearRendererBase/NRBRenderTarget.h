#pragma once
#include <NuclearRendererBase\NRBTexture2D.h>
namespace NuclearRenderer {

	class NRBRenderTarget
	{
		virtual void Create() = 0;

		virtual void AttachDepthBuffer(unsigned int x, unsigned int y) = 0;

		virtual void AttachStencilBuffer(unsigned int x, unsigned int y) = 0;

		virtual void AttachDepthStencilBuffer(unsigned int x, unsigned int y) = 0;

		virtual void AttachTexture(NRBTexture2D& texture) = 0;

		virtual void DetachAll() = 0;

		virtual void Bind() =0;

		virtual void Unbind() = 0;
	};
}