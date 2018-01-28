#include <Core\Context.h>
#include <Core\Engine.h>

#ifndef NE_USE_RUNTIME_RENDER_API
#ifdef NE_USE_OPENGL_3_3
#include <API\OpenGL\GLContext.h>
static NuclearEngine::API::OpenGL::GLContext ctx;
#endif

#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11Context.h>
static NuclearEngine::API::DirectX::DX11Context ctx;
#endif

#else
#include <API\OpenGL\GLContext.h>
#include <API\DirectX\DX11Context.h>
#include "..\..\include\Core\Context.h"

#endif


namespace NuclearEngine
{
	namespace Core
	{
		static RenderAPI _renderer;

		bool Context::Initialize(const RenderAPI & renderer)
		{
			_renderer = renderer;

#ifndef NE_USE_RUNTIME_RENDER_API
			return ctx.Initialize();
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::Initialize();
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::Initialize();
			}
#endif
			return false;
		}

		RenderAPI Context::GetRenderAPI()
		{
			return _renderer;
		}

		void Context::SetPrimitiveType(PrimitiveType primitivetype)
		{
#ifndef NE_USE_RUNTIME_RENDER_API
			ctx.SetPrimitiveType(primitivetype);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::SetPrimitiveType(primitivetype);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::SetPrimitiveType(primitivetype);
			}
#endif
		}

		void Context::Clear(API::Color color, uint flags, float depth, unsigned int stencil)
		{
#ifndef NE_USE_RUNTIME_RENDER_API
			ctx.Clear(color, flags, depth, stencil);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::Clear(color , flags, depth , stencil);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::Clear(color, flags, depth, stencil);
			}
#endif
		}

		void Context::Begin()
		{
			Core::Engine::GetWindow().ProcessEvents();
		}

		void Context::End()
		{
			Core::Engine::GetWindow().SwapBuffers();
		}

		void Context::EnableDepthBuffer(bool state)
		{
#ifndef NE_USE_RUNTIME_RENDER_API
			ctx.EnableDepthBuffer(state);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::EnableDepthBuffer(state);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::EnableDepthBuffer(state);
			}
#endif
		}

		void Context::Draw(unsigned int count)
		{
#ifndef NE_USE_RUNTIME_RENDER_API
			ctx.Draw(count);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::Draw(count);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::Draw(count);
			}
#endif
		}

		void Context::DrawIndexed(unsigned int vertexCount)
		{
#ifndef NE_USE_RUNTIME_RENDER_API
			ctx.DrawIndexed(vertexCount);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::DrawIndexed(vertexCount);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::DrawIndexed(vertexCount);
			}
#endif
		}

		void Context::SetViewPort(int x, int y, int width, int height)
		{
#ifndef NE_USE_RUNTIME_RENDER_API
			ctx.SetViewPort(x, y, width, height);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return API::OpenGL::GLContext::SetViewPort(x, y, width, height);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return API::DirectX::DX11Context::SetViewPort(x, y, width, height);
			}
#endif
		}
	}
}