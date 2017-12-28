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

		i = 0;
	}
	void GLRenderTarget::Delete()
	{
		glDeleteFramebuffers(1, &FBO);

		i = 0;
	}
	void GLRenderTarget::AttachDepthBuffer(NRBTexture* texture)
	{
//		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->GLGetTextureID(), 0);
	}
	void GLRenderTarget::AttachStencilBuffer(NRBTexture* texture)
	{
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->GLGetTextureID(), 0);
	}
	void GLRenderTarget::AttachDepthStencilBuffer(NRBTexture* texture)
	{
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->GLGetTextureID(), 0);
	}
	void GLRenderTarget::AttachTexture(NRBTexture* texture)
	{
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->GLGetTextureID(), 0);
		i++;
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