#pragma once
#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Audio { class Channel; }

	namespace Assets
	{
		class NEAPI AudioClip : public IAsset
		{
		public:
			AudioClip();
			~AudioClip();

		private:
			Uint32 mOALid;
		};
	}
}