#pragma once
#include <NE_Common.h>
#include <Assets/SavedScene.h>
#include <Assets/AssetLibrary.h>

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

			bool SaveScene();

			bool SaveAssetLibraryInfo(Assets::AssetLibrary* lib);

		private:
			SerializationEngine();
		};

	}
}