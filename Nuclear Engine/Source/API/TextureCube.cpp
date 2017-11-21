#include "..\..\include\API\TextureCube.h"
#include <Core\Context.h>
#include <NuclearRendererBase\NRBTextureCube.h>


namespace NuclearEngine {
	namespace API {
		TextureCube::TextureCube(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc)
		{
			tex = Core::Context::ConstructTextureCube(tex);

			tex->Create(data, Desc);
		}
		TextureCube::~TextureCube()
		{
			tex->Delete();

			delete tex;
		}
		void TextureCube::Bind(unsigned int index)
		{
			tex->Bind(index);
		}
		void TextureCube::Bind(const char * samplerName, Shader * shader, unsigned int index)
		{
			tex->Bind(samplerName, shader->GetBase(), index);

		}
		void TextureCube::Unbind()
		{
			tex->Unbind();
		}
		void TextureCube::Unbind(unsigned int index)
		{
			tex->Unbind(index);
		}
	}
}