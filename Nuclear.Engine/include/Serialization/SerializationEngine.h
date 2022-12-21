#pragma once
#include <NE_Common.h>
#include <Serialization/BinaryBuffer.h>
#include <Assets/SavedScene.h>
#include <Assets/AssetLibrary.h>
#include <Assets/AssetMetadata.h>

namespace Nuclear
{
	namespace Serialization
	{		
		class NEAPI SerializationEngine
		{
		public:
			static SerializationEngine& GetInstance();

			SerializationEngine(const SerializationEngine&) = delete;
			SerializationEngine& operator= (const SerializationEngine) = delete;

			bool Serialize(const Assets::AssetMetadata& metadata, BinaryBuffer& outbuffer);

			bool Deserialize(Assets::AssetMetadata& inmetadata, const BinaryBuffer& buffer);

			bool SaveScene();

			bool SaveAssetLibraryInfo(Assets::AssetLibrary* lib);

		private:
			SerializationEngine();
		};

	}
}