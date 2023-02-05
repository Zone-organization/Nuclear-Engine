#pragma once
#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Assets
	{
		class NEAPI AudioClip : public IAsset
		{
		public:
			AudioClip();
			~AudioClip();

		//private:
			Uint32 mBufferID;
		};
	}
}