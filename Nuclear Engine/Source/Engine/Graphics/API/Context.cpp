#include <Engine\Graphics\API\Context.h>
#include <Core\Engine.h>
#include <Core\Application.h>
#include <Engine/Graphics/API/RenderAPI.h>

#ifndef NE_USE_RUNTIME_RENDER_API
#ifdef NE_USE_OPENGL_3_3
#include <Engine/Graphics/API/OpenGL\GLContext.h>
static NuclearEngine::Graphics::API::OpenGL::GLContext ctx;
#endif

#ifdef NE_USE_DIRECTX11
#include <Engine/Graphics/API/DirectX\DX11Context.h>
static NuclearEngine::Graphics::API::DirectX::DX11Context ctx;
#endif

#else
#include <Engine/Graphics/API/OpenGL\GLContext.h>
#include <Engine/Graphics/API/DirectX\DX11Context.h>
#include <Engine/Graphics/API/Context.h>
#endif

#ifndef NE_USE_RUNTIME_RENDER_API
#define CONTEXT_API_GET_FUNC_CALL(FuncName) \
return ctx.##FuncName();
#else
#define CONTEXT_API_GET_FUNC_CALL(ClassName) \
if (Graphics::API::Context::isOpenGL3RenderAPI()) {	return Graphics::API::OpenGL::GLContext::Get##ClassName(); } \
else if (Graphics::API::Context::isDirectX11RenderAPI()) { return Graphics::API::DirectX::DX11Context::Get##ClassName();}
#endif


namespace NuclearEngine
{
	using namespace Core;
	namespace Graphics
	{
		namespace API {
			static RenderAPI _renderer;
			static bool isOpengl3 = false;
			static bool isDirectX11 = false;;
			RenderAPI Context::GetRenderAPI()
			{
				return _renderer;
			}

			void Context::SetPrimitiveType(PrimitiveType primitivetype)
			{
#ifndef NE_USE_RUNTIME_RENDER_API
				ctx.SetPrimitiveType(primitivetype);
#else
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::SetPrimitiveType(primitivetype);
				}
				else if (_renderer == RenderAPI::DirectX11)
				{
					return Graphics::API::DirectX::DX11Context::SetPrimitiveType(primitivetype);
				}
#endif
			}

			void Context::Clear(Graphics::Color color, Uint32 flags, float depth, unsigned int stencil)
			{
#ifndef NE_USE_RUNTIME_RENDER_API
				ctx.Clear(color, flags, depth, stencil);
#else
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::Clear(color, flags, depth, stencil);
				}
				else if (_renderer == RenderAPI::DirectX11)
				{
					return Graphics::API::DirectX::DX11Context::Clear(color, flags, depth, stencil);
				}
#endif
			}

			void Context::PresentFrame()
			{
				//First Render the frame then go check events
				Core::Application::SwapBuffers();
				Core::Application::PollEvents();
			}

			void Context::Draw(unsigned int count)
			{
#ifndef NE_USE_RUNTIME_RENDER_API
				ctx.Draw(count);
#else
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::Draw(count);
				}
				else if (_renderer == RenderAPI::DirectX11)
				{
					return Graphics::API::DirectX::DX11Context::Draw(count);
				}
#endif
			}

			void Context::DrawIndexed(unsigned int vertexCount, unsigned int StartIndexLocation, int BaseVertexLocation )
			{
#ifndef NE_USE_RUNTIME_RENDER_API
				ctx.DrawIndexed(vertexCount);
#else
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::DrawIndexed(vertexCount, StartIndexLocation, BaseVertexLocation);
				}
				else if (_renderer == RenderAPI::DirectX11)
				{
					return Graphics::API::DirectX::DX11Context::DrawIndexed(vertexCount, StartIndexLocation, BaseVertexLocation);
				}
#endif
			}		
			
			static IndicesFormat gFormat = IndicesFormat::UINT_R32;

			void Context::SetIndicesType(const IndicesFormat & type)
			{
				gFormat = type;
#ifdef NE_COMPILE_CORE_OPENGL
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::SetIndicesType(type);
				}
#endif
			}

			IndicesFormat Context::GetIndicesType()
			{
				return gFormat;
			}

			void Context::SetViewPort(int x, int y, int width, int height)
			{
#ifndef NE_USE_RUNTIME_RENDER_API
				ctx.SetViewPort(x, y, width, height);
#else
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::SetViewPort(x, y, width, height);
				}
				else if (_renderer == RenderAPI::DirectX11)
				{
					return Graphics::API::DirectX::DX11Context::SetViewPort(x, y, width, height);
				}
#endif
			}
			void Context::SetScissors(int x, int y, int width, int height)
			{
#ifndef NE_USE_RUNTIME_RENDER_API
				ctx.SetScissors(x, y, width, height);
#else
				if (_renderer == RenderAPI::OpenGL4_5)
				{
					return Graphics::API::OpenGL::GLContext::SetScissors(x, y, width, height);
				}
				else if (_renderer == RenderAPI::DirectX11)
				{
					return Graphics::API::DirectX::DX11Context::SetScissors(x, y, width, height);
				}
#endif
			}
			bool Context::isOpenGL3RenderAPI()
			{
				return isOpengl3;
			}
			bool Context::isDirectX11RenderAPI()
			{
				return isDirectX11;
			}
			void Context::SetRenderAPI(const RenderAPI & renderer)
			{
				_renderer = renderer;

				if (_renderer == Core::RenderAPI::OpenGL4_5)
				{
					isOpengl3 = true;
					isDirectX11 = false;
				}
				else if (_renderer == Core::RenderAPI::DirectX11)
				{	
					isOpengl3 = false;
					isDirectX11 = true;
				}
			}

			SavedState Context::SaveState()
			{
				SavedState state;
				state.mIndicesFormat = GetIndicesType();
				state.mRasterizerState	=	GetRasterizerState();
				state.mBlendState		=	GetBlendState();
				state.mDepthStencilState=	GetDepthStencilState();
				state.mTexture			=	GetPSTexture();
				state.mSampler			=	GetPSSampler();
				state.mPixelShader		=	GetPixelShader();
				state.mVertexShader		=	GetVertexShader();
				state.mIndexBuffer		=	GetIndexBuffer();
				state.mVertexBuffer		=	GetVertexBuffer();
				state.mConstantBuffer	=	GetConstantBuffer();
				return state;
			}

			void Context::LoadState(SavedState& state)
			{
				SetIndicesType(state.mIndicesFormat);
				state.mRasterizerState.Bind();
				state.mBlendState.Bind(state.mBlendState.SavedBlendFactor, state.mBlendState.SavedSampleMask);
				state.mDepthStencilState.Bind();
				state.mTexture.PSBind(0);
				state.mSampler.PSBind(0);
				state.mPixelShader.Bind();
				state.mVertexShader.Bind();
				state.mIndexBuffer.Bind();
				state.mVertexBuffer.Bind();

			}


			RasterizerState Context::GetRasterizerState()
			{
				CONTEXT_API_GET_FUNC_CALL(RasterizerState)
			}
			BlendState Context::GetBlendState()
			{
				CONTEXT_API_GET_FUNC_CALL(BlendState)
			}
			DepthStencilState Context::GetDepthStencilState()
			{
				CONTEXT_API_GET_FUNC_CALL(DepthStencilState)
			}
			Texture Context::GetPSTexture()
			{
				CONTEXT_API_GET_FUNC_CALL(PSTexture)
			}
			Sampler Context::GetPSSampler()
			{
				CONTEXT_API_GET_FUNC_CALL(PSSampler)
			}
			PixelShader Context::GetPixelShader()
			{
				CONTEXT_API_GET_FUNC_CALL(PixelShader)
			}
			VertexShader Context::GetVertexShader()
			{
				CONTEXT_API_GET_FUNC_CALL(VertexShader)
			}
			IndexBuffer Context::GetIndexBuffer()
			{
				CONTEXT_API_GET_FUNC_CALL(IndexBuffer)
			}
			VertexBuffer Context::GetVertexBuffer()
			{
				CONTEXT_API_GET_FUNC_CALL(VertexBuffer)
			}
			ConstantBuffer Context::GetConstantBuffer()
			{
				CONTEXT_API_GET_FUNC_CALL(ConstantBuffer)
			}
		}
	}
}