#pragma once
#include <NE_Common.h>
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

			bool Serialize(const Assets::AssetMetadata& metadata, const Core::Path& path);

			bool Deserialize(Assets::AssetMetadata& inmetadata, const Core::Path& path);

			bool SaveScene();

			bool SaveAssetLibraryInfo(Assets::AssetLibrary* lib);

		private:
			SerializationEngine();
		};

	}
}