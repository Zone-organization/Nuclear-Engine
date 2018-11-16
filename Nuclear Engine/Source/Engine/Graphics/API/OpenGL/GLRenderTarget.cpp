#include <Engine/Graphics/API/OpenGL\GLRenderTarget.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/OpenGL\GLTexture.h>


namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLRenderTarget::GLRenderTarget()
				{
					FBO = 0;
					RBO = 0;
				}
				GLRenderTarget::~GLRenderTarget()
				{
					FBO = 0;
					RBO = 0;
				}
				void GLRenderTarget::Create(GLRenderTarget * result)
				{
					glGenFramebuffers(1, &result->FBO);

				}
				void GLRenderTarget::Delete(GLRenderTarget * result)
				{
					if (result->FBO != 0)
					{
						glDeleteFramebuffers(1, &result->FBO);
					}
					if (result->RBO != 0)
					{
						glDeleteRenderbuffers(1, &result->RBO);
					}
					result->FBO = 0;
					result->RBO = 0;
				}
				void GLRenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
				{
					if (CreateRenderBuffer(GL_DEPTH24_STENCIL8, size) == true)
					{
						glBindFramebuffer(GL_FRAMEBUFFER, FBO);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
					}
				}
				void GLRenderTarget::AttachTexture(GLTexture * texture, const Texture_Desc& Desc)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);

					switch (Desc.Type)
					{
					case TextureType::Texture2D:
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorAttachmentsi, GL_TEXTURE_2D, texture->GLGetTextureID(), 0);
						ColorAttachmentsi++;
					default:
						Exceptions::NotImplementedException();
					}
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				void GLRenderTarget::Bind()
				{
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);
				}
				void GLRenderTarget::Bind_Default()
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				bool GLRenderTarget::CreateRenderBuffer(GLenum format, const Math::Vector2ui& size)
				{
					//Check if has been created before
					if (RBO != 0)
					{
						Log.Error("[GLRenderTarget] RenderTarget (ID: " + std::to_string(FBO) + ") has DepthStencil Attachement already!\n");
						return false;
					}
					glGenRenderbuffers(1, &RBO);
					glBindRenderbuffer(GL_RENDERBUFFER, RBO);
					glRenderbufferStorage(GL_RENDERBUFFER, format, size.x, size.y);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					return true;
				}
			}
		}
	}
}
#endif