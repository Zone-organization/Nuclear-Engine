#pragma once
#include <Assets/IAsset.h>
#include <Assets/LoadingDescs.h>

namespace Nuclear
{
	namespace Assets
	{	

		//Global serializable asset desc (sort-of)
		//Not stored in the memory during engine running
		//used to import assets that have been 'exported' from the engine.
		//serialized as toml file.
		struct AssetMetadata
		{
			//Important for loading assets
			AssetType mType;
			Utilities::UUID mUUID;
			std::string mName;
			Uint32 mHashedName = 0;

			Uint32 mHashedPath = 0;
			AssetLoadingDesc* pLoadingDesc = nullptr;
		};
	}
}