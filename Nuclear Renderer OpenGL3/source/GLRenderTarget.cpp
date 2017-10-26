#include "..\include\NuclearRendererOGL3\GLRenderTarget.h"

namespace NuclearRenderer
{
	GLRenderTarget::GLRenderTarget()
	{
	}
	void GLRenderTarget::Create()
	{
		unsigned int FBO;
		glGenFramebuffers(1, &FBO);
	}
	void GLRenderTarget::Delete()
	{
		glDeleteFramebuffers(1, &FBO);
	}
	void GLRenderTarget::AttachDepthBuffer(unsigned int x, unsigned int y)
	{
	}
	void GLRenderTarget::AttachStencilBuffer(unsigned int x, unsigned int y)
	{
	}
	void GLRenderTarget::AttachDepthStencilBuffer(unsigned int x, unsigned int y)
	{
	}
	void GLRenderTarget::AttachTexture(NRBTexture2D* texture)
	{
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GL_GetID(), 0);
	}
	void GLRenderTarget::DetachAll()
	{
	}
	void GLRenderTarget::Bind()
	{
		
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}
	void GLRenderTarget::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}