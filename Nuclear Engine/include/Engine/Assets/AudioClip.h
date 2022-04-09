#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets/Common.h>

namespace FMOD
{
	class Sound;
	class Channel;
}

namespace NuclearEngine
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