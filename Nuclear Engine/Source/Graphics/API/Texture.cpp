#include <Graphics/API/Texture.h>
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
			void Texture::Create(Texture *obj, const Texture_Data& TexData, const Texture_Desc& Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, TexData, Desc)
			}

			void Texture::Create(Texture* obj, const std::array<Texture_Data, 6>& data, const Texture_Desc & Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create,data, Desc)
			}

			void Texture::Delete(Texture * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}
			void Texture::VSBind(unsigned int index)
			{
				BASE_API_FUNC_CALL_ARGS(VSBind, index)		
			}

			void Texture::PSBind(unsigned int index)
			{
				BASE_API_FUNC_CALL_ARGS(PSBind, index)
			}
			void Texture::GSBind(unsigned int index)
			{
				BASE_API_FUNC_CALL_ARGS(GSBind, index)
			}
		}
	}
}
