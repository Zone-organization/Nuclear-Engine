#include <API\OpenGL\GLContext.h>


#ifdef NE_COMPILE_CORE_OPENGL
static GLuint GL_PRIMITIVE_TYPE;
#include <API\OpenGL\GLError.h>

#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include "Platform\Win32\Win32_OGL.h"
#endif

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			
			bool GLContext::Initialize()
			{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
				return Internals::Win32_OGL::Initialize();
#endif
			}

			void GLContext::SetPrimitiveType(int primitivetype)
			{
				switch (primitivetype)
				{
				case 0:
				{
					GL_PRIMITIVE_TYPE = GL_POINTS;
					break;
				}
				case 1:
				{
					GL_PRIMITIVE_TYPE = GL_LINES;
					break;
				}
				case 2:
				{
					GL_PRIMITIVE_TYPE = GL_LINE_STRIP;
					break;
				}
				case 3:
				{
					GL_PRIMITIVE_TYPE = GL_TRIANGLES;
					break;
				}
				case 4:
				{
					GL_PRIMITIVE_TYPE = GL_TRIANGLE_STRIP;
					break;
				}
				default:
					break;
				}
			}
						
			void GLContext::Clear(API::Color color, uint flags, float depth, float stencil)
			{
				GLbitfield flaggl = 0;
				//Color buffer
				if (CHECK_BIT(flags, 0))
				{
					GLCall(glClearColor(color.r, color.g, color.b, color.a));
					flaggl = flaggl | GL_COLOR_BUFFER_BIT;
				}
				//Depth Buffer
				if (CHECK_BIT(flags, 1))
				{
					GLCall(glClearDepth(depth));
					flaggl = flaggl | GL_DEPTH_BUFFER_BIT;
				}
				//Stencil Buffer
				if (CHECK_BIT(flags, 2))
				{
					GLCall(glClearStencil(stencil));
					flaggl = flaggl | GL_STENCIL_BUFFER_BIT;
				}
				GLCall(glClear(flaggl));
			}

			void GLContext::EnableDepthBuffer(bool state)
			{
				if (state)
				{
					GLCall(glEnable(GL_DEPTH_TEST));
				}
				else
				{
					GLCall(glDisable(GL_DEPTH_TEST));
				}
			}

			void GLContext::SwapBuffers()
			{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
				return Internals::Win32_OGL::SwapBuffer();
#endif
			}

			void GLContext::Shutdown()
			{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
				return Internals::Win32_OGL::Shutdown();
#endif
			}

			void GLContext::Draw(unsigned int count)
			{
				GLCall(glDrawArrays(GL_PRIMITIVE_TYPE, 0, count));

			}

			void GLContext::DrawIndexed(unsigned int vertexCount)
			{
				GLCall(glDrawElements(GL_PRIMITIVE_TYPE, vertexCount, GL_UNSIGNED_INT, 0));
			}


			void GLContext::SetViewPort(int x, int y, int width, int height)
			{
				GLCall(glViewport(x, y, width, height));
			}

		}
	}

}

#endif