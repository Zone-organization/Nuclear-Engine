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
		void Texture2D::Bind(unsigned int index)
		{
			tex->Bind(index);
		}
		void Texture2D::Bind(const char * samplerName, API::Shader* shader, unsigned int index)
		{
			tex->Bind(samplerName, shader->GetBase(), index);
		}

		void Texture2D::Unbind()
		{
			tex->Unbind();
		}
		void Texture2D::Unbind(unsigned int index)
		{
			tex->Unbind(index);
		}
		NuclearRenderer::NRBTexture2D * Texture2D::GetBase()
		{
			return tex;
		}
	}
}