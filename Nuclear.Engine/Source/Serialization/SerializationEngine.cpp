#include "Serialization/SerializationEngine.h"
#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/string.h>
#include <bitsery/traits/array.h>
#include <Parsers/INIParser.h>
#include <ThirdParty/magic_enum.hpp>
#include <Utilities/Hash.h>

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

		bool SerializationEngine::Serialize(const Assets::AssetMetadata& metadata, const Core::Path& path)
		{
			Parsers::INIFile file(path.GetRealPath());

			Parsers::INIStructure meta;
			meta["General"]["Type"] = "AssetMetadata";

			meta["AssetMetadata"]["Name"] = metadata.mName;
			meta["AssetMetadata"]["UUID"] = metadata.mUUID.str();
			meta["AssetMetadata"]["AssetType"] = magic_enum::enum_name(metadata.mType);
			meta["AssetMetadata"]["HashedName"] = Utilities::int_to_hex<Uint32>(metadata.mHashedName);
			meta["AssetMetadata"]["HashedPath"] = Utilities::int_to_hex<Uint32>(metadata.mHashedPath);

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
				metadata.mUUID = Utilities::UUID(meta["AssetMetadata"]["UUID"]);
				metadata.mType = magic_enum::enum_cast<Assets::AssetType>(meta["AssetMetadata"]["AssetType"]).value_or(Assets::AssetType::Unknown);
				metadata.mHashedName = Utilities::hex_to_uint32(meta["AssetMetadata"]["HashedName"]);
				metadata.mHashedPath = Utilities::hex_to_uint32(meta["AssetMetadata"]["HashedPath"]);
			}	

			return result;
		}
		bool SerializationEngine::SaveScene()
		{
			return false;
		}
		bool SerializationEngine::SaveAssetLibraryInfo(Assets::AssetLibrary* lib)
		{
			return false;
		}
		SerializationEngine::SerializationEngine()
		{
		}
	}
}