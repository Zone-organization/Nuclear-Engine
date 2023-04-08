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
					self.pTexture = static_cast<Texture*>(Serialization::SerializationModule::Get().DeserializeUUID(AssetType::Texture, uuid));
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