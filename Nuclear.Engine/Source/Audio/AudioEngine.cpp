#include <Audio\AudioEngine.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Audio
	{	      
		AudioEngine::AudioEngine()
		{

		}

		bool AudioEngine::Initialize(const AudioEngineDesc& desc)
		{
			/*FMOD_RESULT result;
			result = FMOD::System_Create(&pSystem);

			if (result != FMOD_OK)
			{
				NUCLEAR_ERROR("[AudioEngine] Failed to create FMOD SoundSystem! Info: '{0}'",  FMOD_ErrorString(result));
				return false;
			}


			result = pSystem->init(desc.MaxChannels, FMOD_INIT_NORMAL, 0);
			if (result != FMOD_OK)
			{
				NUCLEAR_ERROR("[AudioEngine] Failed to initialize FMOD SoundSystem! Info: '{0}'", FMOD_ErrorString(result));
				return false;
			}
			NUCLEAR_INFO("[AudioEngine] Initialized FMOD SoundSystem.");
			return true;*/
			return true;
		}
		AudioEngine& AudioEngine::GetInstance()
		{
			static AudioEngine audioengine;

			return audioengine;
		}
	
		void AudioEngine::Shutdown()
		{

		}

		void AudioEngine::Update()
		{
			
		}
	}
}