#include <Graphics/API/Texture.h>
#include <Graphics\API\Context.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>
#include <Graphics\GraphicsEngine.h>
#include <Utilities/Hash.h>

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
				obj->mDimensions.x = TexData.Width;
				obj->mDimensions.y = TexData.Height;
				obj->mHashedName = TexData.HashedName;
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, TexData, Desc)

				obj->isValid = true;				
				for (auto func : *GraphicsEngine::GetTextureCreationCallbacks())
				{
					if (!func.isNull())
						func(obj, TexData, Desc);
				}
			}

			void Texture::Create(Texture* obj, const std::array<Texture_Data, 6>& data, const Texture_Desc & Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create,data, Desc)
			}

			void Texture::Delete(Texture * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}

			Math::Vector2ui Texture::GetDimensions()
			{
				return mDimensions;
			}

			unsigned int Texture::GetWidth()
			{
				return mDimensions.x;
			}
			unsigned int Texture::GetHeight()
			{
				return mDimensions.y;
			}

			Uint32 Texture::GetHashedName()
			{
				return mHashedName;
			}

			Uint8 Texture::GetUsageType()
			{
				return mUsageType;
			}

			void Texture::SetHashedName(Uint32 name)
			{
				mHashedName = name;
			}

			void Texture::SetName(const std::string & str)
			{
				mHashedName = Utilities::Hash(str);
			}

			void Texture::SetUsageType(Uint8 type)
			{
				mUsageType = type;
			}
			void Texture::SetUsageType(TextureUsageType type)
			{
				mUsageType = type;
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
