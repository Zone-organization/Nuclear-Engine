#include <Graphics/API/Texture.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			Texture::Texture()
			{
			}

			Texture::~Texture()
			{
			}
			void Texture::Create(Texture *result, const Texture_Data& TexData, const Texture_Desc& Desc)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLTexture::Create(&result->GLObject, TexData, Desc);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11Texture::Create(&result->DXObject, TexData, Desc);
				}
			}

			void Texture::Create(Texture* result, const std::array<Texture_Data, 6>& data, const Texture_Desc & Desc)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLTexture::Create(&result->GLObject, data, Desc);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11Texture::Create(&result->DXObject, data, Desc);
				}
			}

			void Texture::Delete(Texture * result)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLTexture::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11Texture::Delete(&result->DXObject);
				}
			}

			void Texture::VSBind(unsigned int slot)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.VSBind(slot);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.VSBind(slot);
				}
			}

			void Texture::PSBind(unsigned int slot)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.PSBind(slot);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.PSBind(slot);
				}
			}
			void Texture::GSBind(unsigned int slot)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.GSBind(slot);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.GSBind(slot);
				}
			}
		}
	}
}
#endif