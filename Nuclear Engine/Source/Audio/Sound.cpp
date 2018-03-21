#include <Audio\Sound.h>
#include <FMOD\includer.h> 

#ifndef FMOD_NOT_INCLUDED
#include <Audio\AudioSystem.h>
#include <Audio\Channel.h>
#include <FMOD\inc\fmod.hpp>
#include <FMOD\inc\fmod_errors.h>

namespace NuclearEngine
{
	namespace Audio
	{
		bool Sound::Create(const std::string& path, int mode)
		{
			FMOD_RESULT result;

			result = AudioSystem::GetSystem()->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
			sound->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */

			if (result != FMOD_OK)
			{
				Log.Error("[AudioSystem] Failed to create sound from: (" + path +") Info: " + std::string(FMOD_ErrorString(result)) + "\n");
				return false;
			}

			return true;

		}
		void Sound::Play(Channel * channel, bool paused)
		{
			if (channel == nullptr)
			{
				AudioSystem::GetSystem()->playSound(sound, 0, paused, nullptr);
				return;
			}
			AudioSystem::GetSystem()->playSound(sound, 0, paused, channel->GetChannelPtr());
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
			Log.Error("[AudioSystem] Engine Was compiled without FMOD!\n");
			return false;

		}
		void Sound::Play(Channel * channel, bool paused)
		{
			Log.Error("[AudioSystem] Engine Was compiled without FMOD!\n");
		}
	}
}
#endif