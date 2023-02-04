#include "XAudioBackend.h"
#include <Utilities/Logger.h>
#include <Components/AudioSourceComponent.h>

#include <xaudio2.h>

namespace Nuclear
{
	namespace Audio
	{
		class XAudioCallback : public IXAudio2EngineCallback
		{
			COM_DECLSPEC_NOTHROW void OnProcessingPassEnd() override
			{
				return;
			}

			COM_DECLSPEC_NOTHROW void OnProcessingPassStart() override
			{
				return;
			}

			COM_DECLSPEC_NOTHROW void OnCriticalError(HRESULT error) override
			{
				NUCLEAR_ERROR("[XAudioBackend] XAudio2 Critical Error! Code: {0}", error);
			}
		};

		bool XAudioBackend::Initialize()		
		{
			HRESULT hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
			if (FAILED(hr))
			{
				NUCLEAR_ERROR("[XAudioBackend] Failed to initalize XAudio2! Error Code: {0}", hr);
				return false;
			}
			
			static XAudioCallback callbacks;

			pXAudio2->RegisterForCallbacks(&callbacks);

			hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);

			if (FAILED(hr))
			{
				NUCLEAR_ERROR("[XAudioBackend] Failed to Create XAudio2 MasterVoice! Error Code: {0}", hr);
				return false;
			}

			NUCLEAR_INFO("[XAudioBackend] Initialized XAudio2 successfully.");
			return true;
		}
		void XAudioBackend::Shutdown()
		{
			if (pMasterVoice)
			{
				pMasterVoice->DestroyVoice();
				pMasterVoice = nullptr;
			}

			if (pXAudio2)
			{
				pXAudio2->StopEngine();
				pXAudio2->Release();
				pXAudio2 = nullptr;
			}
		}
		bool XAudioBackend::CreateAudioClip(Assets::AudioClip* result, Assets::AudioFile& file)
		{
			return false;
		}

		bool XAudioBackend::CreateAudioSource(Components::AudioSourceComponent* source)
		{
			WAVEFORMATEX  waveFormat = { 0 };
			waveFormat = {};
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nChannels = info.channels;
			waveFormat.nSamplesPerSec = info.samplerate;
			waveFormat.wBitsPerSample = 8;
			waveFormat.nBlockAlign =waveFormat.nChannels *	waveFormat.wBitsPerSample / 8;
			waveFormat.nAvgBytesPerSec =waveFormat.nSamplesPerSec *	waveFormat.nBlockAlign;
			waveFormat.cbSize = sizeof(WAVEFORMATEX);

			auto hr = pXAudio2->CreateSourceVoice(&source->pSourceVoice, &waveFormat);
			
			if (FAILED(hr))
			{
				NUCLEAR_ERROR("[XAudioBackend] Failed to create AudioSourceComponent!, Error Code: {0}", hr);
				return false;
			}

			return true;
		}

		void XAudioBackend::SetAudioSourceClip(Components::AudioSourceComponent* source, Assets::AudioClip* clip)
		{
		}

		void XAudioBackend::Play(Components::AudioSourceComponent* clip)
		{

		}
		void XAudioBackend::Pause(Components::AudioSourceComponent* clip)
		{

		}
		void XAudioBackend::Stop(Components::AudioSourceComponent* clip)
		{
		}
	}
}