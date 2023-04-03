#include <Audio\AudioModule.h>
#include <Utilities/Logger.h>
#include "OpenAL/OpenALBackend.h"
#include "XAudio/XAudioBackend.h"

namespace Nuclear
{
	namespace Audio
	{	      
		AudioModule::AudioModule()
		{
			pBackend = nullptr;
		}

		bool AudioModule::Initialize(const AudioModuleDesc& desc)
		{

			if (desc.mRequestedBackend == AudioModuleDesc::AudioBackendType::XAudio2)
			{
				pBackend = new XAudioBackend();
			}
			else
			{
				pBackend = new OpenALBackend();
			}

			pBackend->Initialize();
	
			NUCLEAR_INFO("[AudioModule] AudioModule has been initialized successfully!.");
			return true;
		}

		AudioBackend* AudioModule::GetBackend()
		{
			return pBackend;
		}
	
		void AudioModule::Shutdown()
		{
			if (pBackend)
			{
				pBackend->Shutdown();
				delete pBackend;
			}
		}

		void AudioModule::Update()
		{
			
		}
	}
}