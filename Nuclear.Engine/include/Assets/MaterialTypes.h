#pragma once
#include <Core/UUID.h>
#include <vector>
#include <string>
#include <Serialization/IsLoading.h>
#include <Serialization/SerializationModule.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;

		enum class ShaderTextureType : Uint8
		{
			Unknown = 0,

			MaterialTex = 1,
			RenderTarget = 2,
			RenderingEffect = 3,
			ShadowTex = 4,
			Sprite = 5,

			UNKNOWN_MAX = 255
		};

		enum TextureUsageType : Uint8
		{
			Diffuse = 0,
			Albedo = 0,

			Specular = 1,
			Metallic = 1,

			Normal = 2,

			Roughness = 3,
			AO = 4,

			IrradianceMap = 5,
			PreFilterMap = 6,
			BRDF_LUT = 7,

			ShadowTex = 8,

			Sprite = 9,

			AmbientOcclusion = 10,

			Unknown = 255
		};

		struct MaterialTexture
		{
			Texture* pTexture = nullptr;
			TextureUsageType mUsageType;

			constexpr static auto serialize(auto& archive, auto& self) //load
			{
				ISLOADING(archive)
				{
					Core::UUID uuid;
					auto result = archive(uuid, self.mUsageType);
					self.pTexture = static_cast<Texture*>(Serialization::SerializationModule::GetInstance().DeserializeUUID(AssetType::Texture, uuid));
					return result;
				}
				else
				{
					Core::UUID uuid = self.pTexture->GetUUID();
					return archive(uuid, self.mUsageType);
				}
			}
		};

		struct ShaderTexture
		{
			std::string mName;
			Uint32 mSlot = 0;
			MaterialTexture mTex;

			ShaderTextureType mType = ShaderTextureType::Unknown;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mSlot, self.mTex, self.mType);
			}
		};

		struct MaterialTextureSet
		{
			MaterialTextureSet()
				: mData(std::vector<MaterialTexture>()), mHashedName(0), mName(std::string())
			{

			}

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mData, self.mHashedName, self.mName);
			}

			std::vector<MaterialTexture> mData;
			Uint32 mHashedName;
			std::string mName;
		};

		using ShaderTextureSet = std::vector<ShaderTexture>;
	}
}