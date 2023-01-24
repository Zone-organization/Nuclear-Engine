#include "Serialization/SerializationEngine.h"
#include <ThirdParty/zpp_bits.h>
#include <Parsers/INIParser.h>
#include <ThirdParty/magic_enum.hpp>
#include <Utilities/Hash.h>
#include <Assets/AssetLibrary.h>

namespace Nuclear
{
	namespace Serialization
	{
		inline SerializationEngine& SerializationEngine::GetInstance() {
			static SerializationEngine instance;

			return instance;
		}

		/*
		
		//Important for loading assets
			AssetType mType;
			Utilities::UUID mUUID;
			std::string mName;
			Uint32 mHashedName = 0;

			Uint32 mHashedPath = 0;
			AssetImportingDesc mImportingDesc;			
		*/

		//bool ReadValue(Parsers::INIStructure& meta, std::string section, std::string key, std::string value)
		//{

		//}

		const char* SetBool(bool val)
		{
			if (val)
				return "true";

			return "false";
		}
		bool SerializationEngine::Serialize(const Assets::AssetMetadata& metadata, const Core::Path& path)
		{
			Parsers::INIFile file(path.GetRealPath());

			Parsers::INIStructure meta;
			meta["General"]["Type"] = "AssetMetadata";

			meta["AssetMetadata"]["Name"] = metadata.mName;
			meta["AssetMetadata"]["UUID"] = metadata.mUUID.str();
			meta["AssetMetadata"]["AssetType"] = magic_enum::enum_name(metadata.mType);

			switch (metadata.mType)
			{
			case Assets::AssetType::Texture:
			{
				const auto imagedesc = static_cast<Assets::TextureLoadingDesc*>(metadata.pLoadingDesc);
				meta["ImageLoadingDesc"]["Extension"] = magic_enum::enum_name(imagedesc->mExtension);
				meta["ImageLoadingDesc"]["AsyncLoading"] = SetBool(imagedesc->mAsyncLoading);
			}
			default:
				break;
			}

			return file.generate(meta);
		}


		bool SerializationEngine::Deserialize(Assets::AssetMetadata& metadata, const Core::Path& path)
		{
			Parsers::INIFile file(path.GetRealPath());

			Parsers::INIStructure meta;

			bool result = file.read(meta);

			if (meta["General"]["Type"] == "AssetMetadata")
			{
				metadata.mName = meta["AssetMetadata"]["Name"];
				metadata.mUUID = Core::UUID(meta["AssetMetadata"]["UUID"]);
				metadata.mType = magic_enum::enum_cast<Assets::AssetType>(meta["AssetMetadata"]["AssetType"]).value_or(Assets::AssetType::Unknown);

				switch (metadata.mType)
				{
				case Assets::AssetType::Texture:
				{
					auto imagedesc = static_cast<Assets::TextureLoadingDesc*>(metadata.pLoadingDesc = new Assets::TextureLoadingDesc);
					imagedesc->mExtension = magic_enum::enum_cast<Assets::IMAGE_EXTENSION>(meta["ImageLoadingDesc"]["Extension"]).value_or(Assets::IMAGE_EXTENSION::IMAGE_EXTENSION_UNKNOWN);
					imagedesc->mAsyncLoading = meta["ImageLoadingDesc"].GetBoolean("AsyncLoading", true);
				}
				default:
					break;
				}
			}	

			return result;
		}

		bool SerializationEngine::SaveScene()
		{
			return false;
		}
		Assets::IAsset* SerializationEngine::DeserializeUUID(Assets::AssetType type, const Core::UUID& uuid)
		{
			if (type == Assets::AssetType::Shader)
			{
				return Assets::AssetLibrary::GetInstance().mImportedShaders.GetOrAddAsset(uuid);
			}
			else if (type == Assets::AssetType::Material)
			{
				return Assets::AssetLibrary::GetInstance().mImportedMaterials.GetOrAddAsset(uuid);
			}
			else if (type == Assets::AssetType::Texture)
			{
				return Assets::AssetLibrary::GetInstance().mImportedTextures.GetOrAddAsset(uuid);
			}


			return nullptr;
		}
		SerializationEngine::SerializationEngine()
		{
		}
	}
}