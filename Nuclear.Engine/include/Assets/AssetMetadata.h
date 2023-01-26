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
		//serialized as ini file.
		struct AssetMetadata
		{
			AssetMetadata()
			{
				mType = AssetType::Unknown;
				mName = "";
				pLoadingDesc = nullptr;
			}
			~AssetMetadata()
			{
				mType = AssetType::Unknown;
				mName = "";
				pLoadingDesc = nullptr;
			}
			//Important for loading assets
			AssetType mType;
			Core::UUID mUUID;
			std::string mName;
			AssetLoadingDesc* pLoadingDesc = nullptr;
		};
	}
}