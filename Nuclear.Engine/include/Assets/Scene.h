#pragma once
#include <Assets/IAsset.h>
#include <vector>


namespace Nuclear
{
	namespace Assets
	{
		class NEAPI Scene : public IAsset
		{
		public:
			Scene()			
				: IAsset(AssetType::Scene)
			{

			}
			~Scene()
			{

			}

			std::vector<Uint8> mBinaryBuffer;
		};
	}
}