#include <Audio\AudioEngine.h>
#include <Utilities/Logger.h>
#include "OpenAL/OpenALBackend.h"
#include "XAudio/XAudioBackend.h"

namespace Nuclear
{
	namespace Audio
	{	      
		AudioEngine::AudioEngine()
		{
			pBackend = nullptr;
		}

		bool AudioEngine::Initialize(const AudioEngineDesc& desc)
		{

			if (desc.mRequestedBackend == AudioEngineDesc::AudioBackendType::XAudio2)
			{
				pBackend = new XAudioBackend();
			}
			else
			{
				pBackend = new OpenALBackend();
			}

			pBackend->Initialize();
	
			NUCLEAR_INFO("[AudioEngine] AudioEngine has has been initalized successfully!.");
			return true;
		}
		AudioEngine& AudioEngine::GetInstance()
		{
			static AudioEngine audioengine;

			return audioengine;
		}

		AudioBackend* AudioEngine::GetBackend()
		{
			return pBackend;
		}
	
		void AudioEngine::Shutdown()
		{
			if (pBackend)
			{
				pBackend->Shutdown();
				delete pBackend;
			}
		}

		void AudioEngine::Update()
		{
			
		}
	}
}