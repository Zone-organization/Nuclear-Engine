#include <Engine/Graphics/API/Texture.h>
#include <Engine\Graphics\API\Context.h>
#include <Engine/Graphics/API/VertexShader.h>
#include <Engine/Graphics/API/PixelShader.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Base\Utilities/Hash.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			Texture::Texture()
			{
			}

			Texture::~Texture()
			{
			}
			bool Texture::Create(Texture *obj, const Texture_Data& TexData, const Texture_Desc& Desc)
			{
				obj->mTexDesc = Desc;
				STATIC_CREATE_BASE_API_FUNC_CALL_ARGS(TexData, Desc)

				for (auto func : GraphicsEngine::TextureCreationCallbacks)
				{
					if (!func.isNull())
						func(obj, TexData, Desc);
				}

				return obj->isValid;
			}

			bool Texture::Create(Texture* obj, const std::array<Texture_Data, 6>& data, const Texture_Desc & Desc)
			{
				STATIC_CREATE_BASE_API_FUNC_CALL_ARGS(data, Desc)

				return obj->isValid;
			}

			void Texture::Delete(Texture * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}

			Math::Vector3ui Texture::GetDimensions(Uint8 Mipmaplevel)
			{
				if (Mipmaplevel < 0)
				{
					Log.Error("[Texture] Invalid mipmap index.\n");
					return 	Math::Vector3ui(0, 0, 0);
				}

				BASE_API_RETURN_FUNC_CALL_ARGS(GetDimensions , Mipmaplevel)

				//For Unexpected events
				return Math::Vector3ui(0, 0, 0);
			}

			Texture_Desc Texture::GetTextureDesc()
			{
				return mTexDesc;
			}
		
			void Texture::VSBind(Uint8 slot)
			{
				BASE_API_FUNC_CALL_ARGS(VSBind, slot)
			}

			void Texture::PSBind(Uint8 slot)
			{
				BASE_API_FUNC_CALL_ARGS(PSBind, slot)
			}
			void Texture::GSBind(Uint8 slot)
			{
				BASE_API_FUNC_CALL_ARGS(GSBind, slot)
			}
		}
	}
}
