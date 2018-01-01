#include <Core\Context.h>
#include <Core\Engine.h>

#ifndef NE_USE_RUNTIME_RENDERER
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

static NuclearEngine::API::OpenGL::GLContext glctx;
static NuclearEngine::API::DirectX::DX11Context dxctx;

#endif


namespace NuclearEngine
{
	namespace Core
	{
		static RenderAPI _renderer;

		bool Context::Initialize(const RenderAPI & renderer)
		{
			_renderer = renderer;

#ifndef NE_USE_RUNTIME_RENDERER
			return ctx.Initialize();
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.Initialize();
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.Initialize();
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
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.SetPrimitiveType(primitivetype);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.SetPrimitiveType(primitivetype);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.SetPrimitiveType(primitivetype);
			}
#endif
		}

		void Context::Clear(API::Color color, ClearFlags flags, float depth, float stencil)
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.Clear(color, flags, depth, stencil);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.Clear(color , flags, depth , stencil);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.Clear(color, flags, depth, stencil);
			}
#endif
		}

		void Context::Shutdown()
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.Shutdown();
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.Shutdown();
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.Shutdown();
			}
#endif
		}

		void Context::Begin()
		{
			Core::Engine::ProcessEvents();
		}

		void Context::End()
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.SwapBuffers();
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.SwapBuffers();
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.SwapBuffers();
			}
#endif
		}

		void Context::EnableDepthBuffer(bool state)
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.EnableDepthBuffer(state);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.EnableDepthBuffer(state);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.EnableDepthBuffer(state);
			}
#endif
		}

		void Context::Draw(unsigned int count)
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.Draw(count);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.Draw(count);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.Draw(count);
			}
#endif
		}

		void Context::DrawIndexed(unsigned int vertexCount)
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.DrawIndexed(vertexCount);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.DrawIndexed(vertexCount);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.DrawIndexed(vertexCount);
			}
#endif
		}

		void Context::SetViewPort(int x, int y, int width, int height)
		{
#ifndef NE_USE_RUNTIME_RENDERER
			ctx.SetViewPort(x, y, width, height);
#else
			if (_renderer == RenderAPI::OpenGL3)
			{
				return glctx.SetViewPort(x, y, width, height);
			}
			else if (_renderer == RenderAPI::DirectX11)
			{
				return dxctx.SetViewPort(x, y, width, height);
			}
#endif
		}
	}
}