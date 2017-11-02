#pragma once
#include <NuclearRendererBase\NRBTexture2D.h>
namespace NuclearRenderer {

	class NRBRenderTarget
	{
		virtual void Create() = 0;

		virtual void Delete() = 0;

		virtual void AttachDepthBuffer(NRBTexture2D* texture) = 0;

		virtual void AttachStencilBuffer(NRBTexture2D* texture) = 0;

		virtual void AttachDepthStencilBuffer(NRBTexture2D* texture) = 0;

		virtual void AttachTexture(NRBTexture2D* texture) = 0;

		virtual void Bind() =0;

		virtual void Unbind() = 0;
	};
}