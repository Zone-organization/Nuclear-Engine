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
				mHashedName = 0;
				pLoadingDesc = nullptr;
			}
			~AssetMetadata()
			{
				if (pLoadingDesc)
				{
					delete pLoadingDesc;
					pLoadingDesc = nullptr;
				}
			}
			//Important for loading assets
			AssetType mType;
			Utilities::UUID mUUID;
			std::string mName;
			Uint32 mHashedName;
			AssetLoadingDesc* pLoadingDesc = nullptr;
		};
	}
}