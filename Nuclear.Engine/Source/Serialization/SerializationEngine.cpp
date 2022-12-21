#include "Serialization/SerializationEngine.h"
#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/string.h>
#include <bitsery/traits/array.h>
#include <Parsers/INIParser.h>

namespace Nuclear
{
	namespace Serialization
	{
		inline SerializationEngine& SerializationEngine::GetInstance() {
			static SerializationEngine instance;

			return instance;
		}

		using OutputAdapter = bitsery::OutputBufferAdapter<BinaryBuffer>;
		using InputAdapter = bitsery::InputBufferAdapter<BinaryBuffer>;

		/*
		
		//Important for loading assets
			AssetType mType;
			Utilities::UUID mUUID;
			std::string mName;
			Uint32 mHashedName = 0;

			Uint32 mHashedPath = 0;
			AssetImportingDesc mImportingDesc;			
		*/

		bool SerializationEngine::Serialize(const Assets::AssetMetadata& metadata, BinaryBuffer& outbuffer)
		{
			Parsers::INIStructure meta;
			meta["General"]["Type"] = "AssetMetadata";

			meta["AssetMetadata"]["Name"] = metadata.mName;
			meta["AssetMetadata"]["UUID"] = metadata.mUUID.str();
			meta["AssetMetadata"]["AssetType"] = "Metadata";
			meta["AssetMetadata"]["HashedName"] = "Metadata";
			meta["AssetMetadata"]["HashedPath"] = "Metadata";


			bitsery::Serializer<OutputAdapter> ser{ outbuffer };
			
			ser.object(metadata);

			return true;
		}
		bool SerializationEngine::Deserialize(Assets::AssetMetadata& metadata, const BinaryBuffer& outbuffer)
		{
			bitsery::Deserializer<InputAdapter> deser{ outbuffer.begin(), outbuffer.size() };
			deser.object(metadata);
			return true;
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