#pragma once
#include <Assets/IAsset.h>
#include <vector>


namespace Nuclear
{
	namespace Assets
	{
		class NEAPI SavedScene : public IAsset
		{
		public:
			SavedScene();
			~SavedScene();

			std::vector<Uint8> mBinaryBuffer;
		};
	}
}