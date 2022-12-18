#pragma once
#include <Assets/IAsset.h>

namespace FMOD
{
	class Sound;
	class Channel;
}

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

		//private:
			FMOD::Sound* mSound;
		};
	}
}