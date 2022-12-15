#include <Assets\AudioClip.h>
//#include <Audio\AudioEngine.h>
//#include <FMOD\inc\fmod.hpp>
//#include "..\Audio\FMODError.h"
//#include <FMOD/inc/fmod.hpp>

namespace Nuclear
{
	namespace Assets
	{
		AudioClip::AudioClip()
			: Asset(AssetType::AudioClip)
		{

		}
		AudioClip::~AudioClip()
		{

		}
		//void AudioClip::Play(FMOD::Channel* channel, bool paused)
		//{
		//	FMODCALL(Audio::AudioEngine::GetSystem()->playSound(mSound, 0, paused, &channel));
		//}
	}
}