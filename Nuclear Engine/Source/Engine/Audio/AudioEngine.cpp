#include <Engine\Audio\AudioEngine.h>
#include <FMOD/inc/fmod.hpp>
#define FMOD_ERRCHK_IMPL
#include "FMODError.h"
#pragma comment (lib, "fmodL_vc.lib")

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


			result = system->init(MaxChannels, FMOD_INIT_NORMAL, 0);
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
		void AudioEngine::Update(FMOD::Channel * channel)
		{
			system->update();

			if (channel)
			{
				bool playing = 0;
				auto result = channel->isPlaying(&playing);
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