#include <Graphics/API/VertexShader.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>
#include <Graphics/API/ConstantBuffer.h>
namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			VertexShader::VertexShader()
			{

			}

			VertexShader::~VertexShader()
			{

			}

			void VertexShader::Create(VertexShader* result, BinaryShaderBlob* Desc)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLVertexShader::Create(&result->GLObject, Desc);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11VertexShader::Create(&result->DXObject, Desc);
				}
			}

			void VertexShader::Delete(VertexShader * result)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLVertexShader::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11VertexShader::Delete(&result->DXObject);
				}
			}

			void VertexShader::SetConstantBuffer(ConstantBuffer * cbuffer)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.SetConstantBuffer(&cbuffer->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.SetConstantBuffer(&cbuffer->DXObject);
				}
			}

			void VertexShader::Bind()
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.Bind();
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.Bind();
				}
			}
		}
	}
}
#endif