#pragma once
#include <NuclearRendererOGL3\GL_Common.h>
#include <NuclearRendererBase/NRBRenderTarget.h>

namespace NuclearRenderer
{
	class NROGL3API GLRenderTarget : public NuclearRenderer::NRBRenderTarget
	{
	public:
		GLRenderTarget();

		void Create();

		void Delete();

		void AttachDepthBuffer(unsigned int x, unsigned int y);

		void AttachStencilBuffer(unsigned int x, unsigned int y);

		void AttachDepthStencilBuffer(unsigned int x, unsigned int y);

		void AttachTexture(NRBTexture2D* texture);

		void DetachAll();

		void Bind();

		void Unbind();

	private:
		GLuint FBO;
	};
}