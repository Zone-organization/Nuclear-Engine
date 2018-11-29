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
					mFBO = 0;
					mRBO = 0;
				}
				GLRenderTarget::~GLRenderTarget()
				{
					mFBO = 0;
					mRBO = 0;
				}
				void GLRenderTarget::Create(GLRenderTarget * result)
				{
					glGenFramebuffers(1, &result->mFBO);

				}
				void GLRenderTarget::Delete(GLRenderTarget * result)
				{
					if (result->mFBO != 0)
					{
						glDeleteFramebuffers(1, &result->mFBO);
					}
					if (result->mRBO != 0)
					{
						glDeleteRenderbuffers(1, &result->mRBO);
					}
					result->mFBO = 0;
					result->mRBO = 0;
				}
				void GLRenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
				{
					if (CreateRenderBuffer(GL_DEPTH24_STENCIL8, size) == true)
					{
						glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
					}
				}
				void GLRenderTarget::AttachTexture(GLTexture * texture, const Texture_Desc& Desc)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

					switch (Desc.Type)
					{
					case TextureType::Texture2D:
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorAttachmentsi, GL_TEXTURE_2D, texture->mTextureID, 0);
						ColorAttachmentsi++;
					default:
						Exceptions::NotImplementedException();
					}
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				void GLRenderTarget::Bind()
				{
					glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
				}
				void GLRenderTarget::Bind_Default()
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				bool GLRenderTarget::CreateRenderBuffer(GLenum format, const Math::Vector2ui& size)
				{
					//Check if has been created before
					if (mRBO != 0)
					{
						Log.Error("[GLRenderTarget] RenderTarget (ID: " + std::to_string(mFBO) + ") has DepthStencil Attachement already!\n");
						return false;
					}
					glGenRenderbuffers(1, &mRBO);
					glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
					glRenderbufferStorage(GL_RENDERBUFFER, format, size.x, size.y);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					return true;
				}
			}
		}
	}
}
#endif