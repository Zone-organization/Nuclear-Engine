#pragma once
#include <NuclearRendererBase\NRBTexture.h>
namespace NuclearRenderer {

	class NRBRenderTarget
	{
		virtual void Create() = 0;

		virtual void Delete() = 0;

		virtual void AttachDepthBuffer(NRBTexture* texture) = 0;

		virtual void AttachStencilBuffer(NRBTexture* texture) = 0;

		virtual void AttachDepthStencilBuffer(NRBTexture* texture) = 0;

		virtual void AttachTexture(NRBTexture* texture) = 0;

		virtual void Bind() =0;

		virtual void Unbind() = 0;
	};
}