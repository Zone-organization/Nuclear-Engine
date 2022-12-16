#pragma once
#include <Assets/Common.h>
#include <vector>


namespace Nuclear
{
	namespace Assets
	{
		class NEAPI SavedScene : public Asset
		{
		public:
			SavedScene();
			~SavedScene();

			std::vector<Uint8> mBinaryBuffer;
		};
	}
}