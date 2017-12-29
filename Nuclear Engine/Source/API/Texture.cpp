#include <API\Texture.h>
#ifdef NE_USE_RUNTIME_RENDERER
#include <Core\Context.h>
#include <API\Shader.h>
namespace NuclearEngine {
	namespace API {
		Texture::Texture()
		{
		}
		
		Texture::~Texture()
		{
		}
		void Texture::Create(Texture *result,Texture_Data * TexData, Texture_Desc * Desc)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLTexture::Create(&result->GLObject,TexData,Desc);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DirectX::DX11Texture::Create(&result->DXObject, TexData, Desc);
			}
		}

		void Texture::Create(Texture* result, const std::array<NuclearEngine::Texture_Data*, 6>& data, NuclearEngine::Texture_Desc * Desc)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLTexture::Create(&result->GLObject,data, Desc);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DirectX::DX11Texture::Create(&result->DXObject, data, Desc);
			}
		}

		void Texture::VSBind(unsigned int index)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.VSBind(index);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.VSBind(index);
			}
		}
		void Texture::VSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.VSBind(samplerName,&shader->GLObject,index);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.VSBind(samplerName, &shader->DXObject, index);
			}
		}
		void Texture::PSBind(unsigned int index)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.PSBind(index);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.PSBind(index);
			}
		}
		void Texture::PSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.PSBind(samplerName, &shader->GLObject, index);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.PSBind(samplerName, &shader->DXObject, index);
			}
		}	
		void Texture::GSBind(unsigned int index)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.GSBind(index);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.GSBind(index);
			}
		}
		void Texture::GSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.GSBind(samplerName, &shader->GLObject, index);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.GSBind(samplerName, &shader->DXObject, index);
			}
		}

	}
}
#endif