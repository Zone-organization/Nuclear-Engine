#include <Engine\Assets\AudioClip.h>
#include <Engine\Audio\AudioEngine.h>
#include <FMOD\inc\fmod.hpp>
#include "..\Audio\FMODError.h"
#include <FMOD/inc/fmod.hpp>

namespace NuclearEngine
{
	namespace Assets
	{
		void AudioClip::Play(FMOD::Channel* channel, bool paused)
		{
			FMODCALL(Audio::AudioEngine::GetSystem()->playSound(mSound, 0, paused, &channel));
		}
	}
}