#include <Audio\AudioSystem.h>
#include <FMOD\includer.h> 

#ifndef FMOD_NOT_INCLUDED
#include <FMOD\inc\fmod.hpp>
#include <FMOD\inc\fmod_errors.h>
#pragma comment (lib, "FMOD/lib/fmodL_vc.lib")

namespace NuclearEngine
{
	namespace Audio
	{
		FMOD::System *system;
	      
		bool AudioSystem::Initialize(unsigned int MaxChannels)
		{
			FMOD_RESULT result;
			result = FMOD::System_Create(&system);

			if (result != FMOD_OK)
			{
				Log.Error("[AudioSystem] Failed to create FMOD SoundSystem! Info: " + std::string(FMOD_ErrorString(result)) + "\n");
				return false;
			}


			result = system->init(MaxChannels, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
			if (result != FMOD_OK)
			{
				Log.Error("[AudioSystem] Failed to initialize FMOD SoundSystem! Info: " + std::string(FMOD_ErrorString(result)) + "\n");
				return false;
			}

			return true;
		}
		FMOD::System * AudioSystem::GetSystem()
		{
			return system;
		}
		void AudioSystem::Update()
		{
			system->update();
		}
	}
}
#else

namespace NuclearEngine
{
	namespace Audio
	{

		bool AudioSystem::Initialize(unsigned int MaxChannels = 512)
		{
			Log.Error("[AudioSystem] Engine Was compiled without FMOD!\n");
			return false;
		}
		FMOD::System * AudioSystem::GetSystem()
		{
			Log.Error("[AudioSystem] Engine Was compiled without FMOD!\n");
			return nullptr;
		}
		void AudioSystem::Update()
		{
			Log.Error("[AudioSystem] Engine Was compiled without FMOD!\n");
		}
	}
}
#endif