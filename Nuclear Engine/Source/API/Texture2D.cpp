#include "API\Texture2D.h"
#include "API\Shader.h"
#include <Core\Context.h>
#include <NuclearRendererBase\NRBTexture2D.h>


namespace NuclearEngine {
	namespace API {
		Texture2D::Texture2D(const Texture_Data& TexData, const Texture_Desc& Desc)
		{
			tex = Core::Context::ConstructTexture2D(tex);
			Data = TexData;
			if (Data.databuf == NULL)
			{
				Log->Warning("[Texture2D] Binary Texture Data Buffer Is Empty! \n");
			}

			tex->Create(Data, Desc);
		}
		Texture2D::~Texture2D()
		{
			tex->Delete();

			delete tex;
		}	
		void Texture2D::VSBind(unsigned int index)
		{
			tex->VSBind(index);
		}
		void Texture2D::VSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->VSBind(samplerName, shader->GetBase(), index);
		}
		void Texture2D::PSBind(unsigned int index)
		{
			tex->PSBind(index);
		}
		void Texture2D::PSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->PSBind(samplerName, shader->GetBase(), index);
		}	
		void Texture2D::GSBind(unsigned int index)
		{
			tex->GSBind(index);
		}
		void Texture2D::GSBind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->GSBind(samplerName, shader->GetBase(), index);
		}

		NuclearRenderer::NRBTexture2D * Texture2D::GetBase()
		{
			return tex;
		}
	}
}