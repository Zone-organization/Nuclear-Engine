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

		void AttachDepthBuffer(NRBTexture2D* texture) override;

		void AttachStencilBuffer(NRBTexture2D* texture) override;

		void AttachDepthStencilBuffer(NRBTexture2D* texture) override;

		void AttachTexture(NRBTexture2D* texture) override;

		void Bind() override;

		void Unbind() override;

	private:
		GLuint FBO;

		GLuint i;
	};
}