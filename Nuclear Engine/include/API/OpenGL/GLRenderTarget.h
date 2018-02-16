#pragma once
#include <API\OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Math\Vector.h>
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			class GLTexture;
			class NEAPI GLRenderTarget
			{
			public:
				GLRenderTarget();
				~GLRenderTarget();

				static void Create(GLRenderTarget* result);
				static void Delete(GLRenderTarget* result);
	
				void AttachDepthStencilBuffer(const Math::Vector2ui& size);
				
				void AttachTexture(GLTexture* texture);

				void Bind();
				static void Bind_Default();
			private:
				bool CreateRenderBuffer(GLenum storage, const Math::Vector2ui& size);

				unsigned int ColorAttachmentsi = 0;

				//Framebuffer
				GLuint FBO;

				//RenderBuffer
				GLuint RBO;
			};
		}
	}
}
#endif