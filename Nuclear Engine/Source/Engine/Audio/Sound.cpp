#include <Engine\Audio\Sound.h>
//#include <FMOD\includer.h> 

#ifdef FMOD_NOT_INCLUDED
#include <Engine\Audio\AudioEngine.h>
#include <Engine\Audio\Channel.h>
#include <FMOD\inc\fmod.hpp>
#include "FMODError.h"

namespace NuclearEngine
{
	namespace Audio
	{
		bool Sound::Create(const std::string& path, int mode)
		{
			Log.Info("[Sound] Loading: " + path + "\n");
			FMODCALL(AudioEngine::GetSystem()->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound));
			FMODCALL(sound->setMode(FMOD_LOOP_OFF));    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */

			return true;

		}
		void Sound::Play(Channel * channel, bool paused)
		{
			if (channel == nullptr)
			{
				FMODCALL(AudioEngine::GetSystem()->playSound(sound, 0, paused, nullptr));
				return;
			}
			FMODCALL(AudioEngine::GetSystem()->playSound(sound, 0, paused, channel->GetChannelPtr()));
		}
	}
}
#else

namespace NuclearEngine
{
	namespace Audio
	{
		bool Sound::Create(const std::string& path, int mode)
		{
			Log.Error("[AudioEngine] Engine Was compiled without FMOD!\n");
			return false;

		}
		void Sound::Play(Channel * channel, bool paused)
		{
			Log.Error("[AudioEngine] Engine Was compiled without FMOD!\n");
		}
	}
}
#endif