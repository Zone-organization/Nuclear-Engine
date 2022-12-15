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
		class NEAPI AudioClip : public Asset
		{
		public:
			AudioClip();
			~AudioClip();

		//private:
			FMOD::Sound* mSound;
		};
	}
}