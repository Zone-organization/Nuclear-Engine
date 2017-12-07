#include "API\Texture.h"
#include "API\Shader.h"
#include <Core\Context.h>
#include <NuclearRendererBase\NRBTexture.h>


namespace NuclearEngine {
	namespace API {
		Texture::Texture(const Texture_Data& TexData, const Texture_Desc& Desc)
		{
			tex = Core::Context::ConstructTexture(tex);
			Data = TexData;
			
			tex->Create(Data, Desc);
		}
		Texture::~Texture()
		{
			tex->Delete();

			delete tex;
		}	
		void Texture::VSBind(unsigned int index)
		{
			tex->VSBind(index);
		}
		void Texture::VSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->VSBind(samplerName, shader->GetBase(), index);
		}
		void Texture::PSBind(unsigned int index)
		{
			tex->PSBind(index);
		}
		void Texture::PSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->PSBind(samplerName, shader->GetBase(), index);
		}	
		void Texture::GSBind(unsigned int index)
		{
			tex->GSBind(index);
		}
		void Texture::GSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->GSBind(samplerName, shader->GetBase(), index);
		}

		NuclearRenderer::NRBTexture * Texture::GetBase()
		{
			return tex;
		}
	}
}