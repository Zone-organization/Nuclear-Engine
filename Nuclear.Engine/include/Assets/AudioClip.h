#pragma once
#include <Assets/Common.h>

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
		class NEAPI AudioClip : public Asset<AudioClip>
		{
		public:
			AudioClip();
			~AudioClip();

		//private:
			FMOD::Sound* mSound;
		};
	}
}