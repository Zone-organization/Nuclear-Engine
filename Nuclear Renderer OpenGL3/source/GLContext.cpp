#include "NuclearRendererOGL3\GLContext.h"
#include <NuclearRendererOGL3\GLVertexBuffer.h>
#include <NuclearRendererOGL3\GLIndexBuffer.h>
#include <NuclearRendererOGL3\GLConstantBuffer.h>
#include <NuclearRendererOGL3\GLRenderTarget.h>
#include <NuclearRendererOGL3\GLDepthStencilState.h>
#include <NuclearRendererOGL3\GLTexture.h>
#include <NuclearRendererOGL3\GLShader.h>
#include "Platform\Win32\Win32_OGL.h"
#include <NuclearCommon\Utilities\Logger.h>
using namespace NuclearCommon;

static GLuint GL_PRIMITIVE_TYPE;

namespace NuclearRenderer
{
	bool GLContext::Initialize()
	{
		return Internals::Win32_OGL::Initialize();
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

	void GLContext::ClearColor(float Red, float Green, float Blue, float Alpha)
	{
		glClearColor(Red, Green, Blue, Alpha);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GLContext::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GLContext::ClearStencilBuffer()
	{
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void GLContext::EnableDepthBuffer(bool state)
	{
		if (state)
		{
			glEnable(GL_DEPTH_TEST);
			glClearDepth(1.0);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLContext::SwapBuffers()
	{
		return Internals::Win32_OGL::SwapBuffer();
	}

	void GLContext::Shutdown()
	{
		return Internals::Win32_OGL::Shutdown();
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

	NRBVertexBuffer * GLContext::ConstructVertexBuffer(NRBVertexBuffer * param)
	{
		param = new NuclearRenderer::GLVertexBuffer();

		return param;
	}

	NRBIndexBuffer * GLContext::ConstructIndexBuffer(NRBIndexBuffer * param)
	{
		param = new NuclearRenderer::GLIndexBuffer();

		return param;
	}

	NRBConstantBuffer * GLContext::ConstructConstantBuffer(NRBConstantBuffer * param)
	{
		param = new NuclearRenderer::GLConstantBuffer();

		return param;
	}

	NRBRenderTarget * GLContext::ConstructRenderTarget(NRBRenderTarget * param)
	{
		param = new NuclearRenderer::GLRenderTarget();

		return param;
	}

	NRBDepthStencilState * GLContext::ConstructDepthStencilState(NRBDepthStencilState * param)
	{
		param = new NuclearRenderer::GLDepthStencilState();

		return param; 
	}

	NRBTexture * GLContext::ConstructTexture(NRBTexture * param)
	{
		param = new NuclearRenderer::GLTexture();

		return param;
	}


	NRBShader* GLContext::ConstructShader(NRBShader * param)
	{
		param = new NuclearRenderer::GLShader();

		return param;
	}
}