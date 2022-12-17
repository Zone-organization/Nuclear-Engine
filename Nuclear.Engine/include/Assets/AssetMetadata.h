#pragma once
#include <Assets/Common.h>

namespace Nuclear
{
	namespace Assets
	{
		struct AssetLoadingDesc {

		};

	

		//Global serializable asset desc (sort-of)
		//Not stored in the memory during engine running
		//used to import assets that have been 'exported' from the engine.
		struct AssetMetadata
		{
			Uint32 mHashedPath = 0;
			Uint32 mHashedName = 0;
			std::string mName;

			AssetType mType;

			AssetLoadingDesc* mLoadingDesc = nullptr;
		};
	}
}