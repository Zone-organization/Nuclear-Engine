#include <Core\Context.h>
#include <Core\Engine.h>
#include <NuclearRendererBase\NRBContext.h>
#include <NuclearRendererBase\NRBVertexBuffer.h>
#include <NuclearRendererBase\NRBIndexBuffer.h>
#include <NuclearRendererBase\NRBUniformBuffer.h>
#include <NuclearRendererBase\NRBTexture2D.h>
#include <NuclearRendererBase\NRBShader.h>
#include <NuclearRendererOGL3\GLContext.h>
#include <NuclearRendererDX11\DX11Context.h>

namespace NuclearEngine
{
	namespace Core 
	{
		static NuclearRenderer::NRBContext *ctx;
		static RenderAPI _renderer;
		bool Context::Initialize(NuclearRenderer::NRBContext * context)
		{
			ctx = context;
			return ctx->Initialize();
		}

		bool Context::Initialize(const RenderAPI & renderer)
		{
			if (renderer == RenderAPI::OpenGL3)
			{
				ctx = new NuclearRenderer::GLContext();
			}
			else if (renderer == RenderAPI::DirectX11)
			{
				ctx = new NuclearRenderer::DX11Context();
			}

			_renderer = renderer;
			return ctx->Initialize();
		}

		RenderAPI Context::GetRenderAPI()
		{
			return _renderer;
		}

		void Context::SetPrimitiveType(PrimitiveType primitivetype)
		{
			ctx->SetPrimitiveType(primitivetype);
		}

		void Context::ClearColor(API::Color color)
		{
			ctx->ClearColor(color.r, color.g, color.b, color.a);
		}

		void Context::Shutdown()
		{
			ctx->Shutdown();
		}

		void Context::Begin()
		{
			Core::Engine::ProcessEvents();
		}

		void Context::End()
		{
			ctx->SwapBuffers();
		}

		void Context::Draw(unsigned int count)
		{
			ctx->Draw(count);

		}

		void Context::DrawIndexed(unsigned int vertexCount)
		{
			ctx->DrawIndexed(vertexCount);

		}

		void Context::EnableDepthBuffer(bool state)
		{
			ctx->EnableDepthBuffer(state);

		}

		void Context::EnableBlending(bool state)
		{
			ctx->EnableBlending(state);

		}

		void Context::SetViewPort(int x, int y, int width, int height)
		{
			ctx->SetViewPort(x, y, width, height);
		}
		using namespace NuclearRenderer;

		NRBVertexBuffer * Context::ConstructVertexBuffer(NRBVertexBuffer * param)
		{
			return ctx->ConstructVertexBuffer(param);
		}

		NRBIndexBuffer * Context::ConstructIndexBuffer(NRBIndexBuffer * param)
		{
			return ctx->ConstructIndexBuffer(param);
		}

		NRBUniformBuffer * Context::ConstructUniformBuffer(NRBUniformBuffer * param)
		{
			return ctx->ConstructUniformBuffer(param);
		}

		NRBTexture2D * Context::ConstructTexture2D(NRBTexture2D * param)
		{
			return ctx->ConstructTexture2D(param);
		}

		NRBShader* Context::ConstructShader(NRBShader * param)
		{
			return ctx->ConstructShader(param);
		}

	}
}