#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
#include <NuclearRendererBase/NRBRenderTarget.h>

namespace NuclearRenderer
{
	class NROGL3API GLRenderTarget : public NuclearRenderer::NRBRenderTarget
	{
	public:
		GLRenderTarget();

		void Create() override;

		void Delete() override;

		void AttachDepthBuffer(NRBTexture* texture) override;

		void AttachStencilBuffer(NRBTexture* texture) override;

		void AttachDepthStencilBuffer(NRBTexture* texture) override;

		void AttachTexture(NRBTexture* texture) override;

		void Bind() override;

		void Unbind() override;

	private:
		GLuint FBO;

		GLuint i;
	};
}