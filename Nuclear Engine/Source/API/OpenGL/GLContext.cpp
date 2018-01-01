#include <API\OpenGL\GLContext.h>


#ifdef NE_COMPILE_OPENGL3_3
static GLuint GL_PRIMITIVE_TYPE;

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

			GLbitfield GL_GetClearFlags(ClearFlags flags)
			{
				switch (flags)
				{
				case ClearFlags::None:
					return GL_COLOR_BUFFER_BIT;
				case ClearFlags::Depth:
					return (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				case ClearFlags::Stencil:
					return(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				case ClearFlags::Depth_Stencil:
					return (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				}

				return GL_COLOR_BUFFER_BIT;
			}

			void GLContext::Clear(API::Color color, ClearFlags flags, float depth, float stencil)
			{
				glClearColor(color.r, color.g, color.b, color.a);
				glClear(GL_GetClearFlags(flags));
				glClearDepth(depth);
				glClearStencil(stencil);
			}

			void GLContext::EnableDepthBuffer(bool state)
			{
				if (state)
				{
					glEnable(GL_DEPTH_TEST);
				}
				else
				{
					glDisable(GL_DEPTH_TEST);
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
				glDrawArrays(GL_PRIMITIVE_TYPE, 0, count);

			}

			void GLContext::DrawIndexed(unsigned int vertexCount)
			{
				glDrawElements(GL_PRIMITIVE_TYPE, vertexCount, GL_UNSIGNED_INT, 0);
			}


			void GLContext::SetViewPort(int x, int y, int width, int height)
			{
				glViewport(x, y, width, height);
			}

		}
	}

}

#endif