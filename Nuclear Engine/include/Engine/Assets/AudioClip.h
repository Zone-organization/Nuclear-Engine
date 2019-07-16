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
		class NEAPI AudioClip 
		{
		public:

			void Play(FMOD::Channel* channel = nullptr, bool paused = false);

			FMOD::Sound* mSound;
		};
	}
}