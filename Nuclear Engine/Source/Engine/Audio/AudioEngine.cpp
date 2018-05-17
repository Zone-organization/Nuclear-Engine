#include <Audio\AudioEngine.h>
#include <FMOD\includer.h> 

#ifndef FMOD_NOT_INCLUDED
#include <FMOD\inc\fmod.hpp>
#define FMOD_ERRCHK_IMPL
#include "FMODError.h"
#pragma comment (lib, "FMOD/lib/fmodL_vc.lib")
#include <Audio\Channel.h>

namespace NuclearEngine
{
	namespace Audio
	{
		FMOD::System *system;
	      
		bool AudioEngine::Initialize(unsigned int MaxChannels)
		{
			FMOD_RESULT result;
			result = FMOD::System_Create(&system);

			if (result != FMOD_OK)
			{
				Log.Error("[AudioEngine] Failed to create FMOD SoundSystem! Info: " + std::string(FMOD_ErrorString(result)) + "\n");
				return false;
			}


			result = system->init(MaxChannels, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
			if (result != FMOD_OK)
			{
				Log.Error("[AudioEngine] Failed to initialize FMOD SoundSystem! Info: " + std::string(FMOD_ErrorString(result)) + "\n");
				return false;
			}
			Log.Info("[AudioEngine] Initialized FMOD SoundSystem.\n");
			return true;
		}
		void AudioEngine::Shutdown()
		{
			system->close();
			system->release();
		}
		FMOD::System * AudioEngine::GetSystem()
		{
			return system;
		}
		void AudioEngine::Update(Channel * channel)
		{
			system->update();

			if (channel)
			{
				bool playing = 0;
				auto result = channel->GetChannel()->isPlaying(&playing);
				if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
				{
					if (result != FMOD_OK)
					{
						Log.Error("[AudioEngine] FMOD Error: " + std::string(FMOD_ErrorString(result)) + "\n");
						return;
					}
				}

			}
		}
	}
}
#else

namespace NuclearEngine
{
	namespace Audio
	{

		bool AudioEngine::Initialize(unsigned int MaxChannels)
		{
			Log.Error("[AudioEngine] Engine Was compiled without FMOD!\n");
			return false;
		}
		FMOD::System * AudioEngine::GetSystem()
		{
			return nullptr;
		}
		void AudioEngine::Shutdown()
		{
		}
		void AudioEngine::Update(Channel * channel)
		{
		}
	}
}
#endif