#include "XAudioBackend.h"
#include <Utilities/Logger.h>
#include <Components/AudioSourceComponent.h>
#include <Assets/AudioClip.h>

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

			//TODO: this number should be modifiable in initialization;
			mSources.reserve(8);

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
		bool XAudioBackend::CreateAudioClip(Assets::AudioClip* result, AudioFile& file)
		{
			bool match = false;
			for (Uint32 i = 0; i < mBuffers.size(); i++)
			{
				auto& buffer = GetBuffer(i);
				if (!buffer.Used)
				{
					buffer.mFile.mData = std::move(file.mData);
					buffer.mFile.mInfo = file.mInfo;
					buffer.Used = true;
					match = true;
					break;
				}
			}

			if (!match)
			{
				XAudioBuffer buffer;
				buffer.mFile.mData = std::move(file.mData);
				buffer.mFile.mInfo = file.mInfo;
				buffer.Used = true;
				mBuffers.push_back(buffer);
				result->mBufferID = mBuffers.size();

				//should probably notify engine/allocate more buffers
			}


			return false;
		}

		bool XAudioBackend::CreateAudioSource(Components::AudioSourceComponent* comp)
		{
			auto& xbuffer = GetBuffer(comp->pActiveClip->mBufferID);
			if (!xbuffer.Used)
			{
				return false;
			}

			WAVEFORMATEX  waveFormat = { 0 };
			waveFormat = {};
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nChannels = xbuffer.mFile.mInfo.mChannels;
			waveFormat.nSamplesPerSec = xbuffer.mFile.mInfo.mSampleRate;
			waveFormat.wBitsPerSample = xbuffer.mFile.mInfo.mBitsPerSample;
			waveFormat.nBlockAlign = waveFormat.nChannels *	waveFormat.wBitsPerSample / 8;
			waveFormat.nAvgBytesPerSec =waveFormat.nSamplesPerSec *	waveFormat.nBlockAlign;
			waveFormat.cbSize = sizeof(WAVEFORMATEX);

			IXAudio2SourceVoice* voice;
			auto hr = pXAudio2->CreateSourceVoice(&voice, &waveFormat);
			
			if (FAILED(hr))
			{
				NUCLEAR_ERROR("[XAudioBackend] Failed to create AudioSourceComponent!, Error Code: {0}", hr);
				return false;
			}

			bool match = false;
			for (Uint32 i = 0; i < mSources.size(); i++)
			{
				auto& source = GetSource(i);
				if (source.Unused())
				{
					comp->mSourceID = i;
					source.pVoice = voice;
					match = true;
					break;
				}
			}

			if (!match)
			{
				XAudioSource source;
				source.pVoice = voice;
				mSources.push_back(source);
				comp->mSourceID = mSources.size();

				//should probably notify engine/allocate more sources
			}

			return true;
		}

		void XAudioBackend::SetAudioSourceClip(Components::AudioSourceComponent* source, Assets::AudioClip* clip)
		{
			auto& xsource = GetSource(source->mSourceID);
			if (xsource.Unused())
			{
				return;
			}

			auto& xbuffer = GetBuffer(clip->mBufferID);
			if (!xbuffer.Used)
			{
				return;
			}

			XAUDIO2_BUFFER buffer = { 0 };
			buffer.AudioBytes = (UINT32)xbuffer.mFile.mData.size();
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopBegin = 0;
			buffer.LoopCount = 0;
			buffer.LoopLength = 0;
			buffer.pAudioData = xbuffer.mFile.mData.data();
			buffer.pContext = nullptr;
			buffer.PlayBegin = 0;
			buffer.PlayLength = (UINT32)xbuffer.mFile.mInfo.mSamples;

			auto hr = xsource.pVoice->SubmitSourceBuffer(&buffer);

			if (FAILED(hr))
			{
				NUCLEAR_ERROR("[XAudioBackend] Failed to SubmitSourceBuffer!, Error Code: {0}", hr);
			}

			return;
		}

		void XAudioBackend::Play(Components::AudioSourceComponent* clip)
		{
			GetSource(clip->mSourceID).pVoice->Start(0);
		}
		void XAudioBackend::Pause(Components::AudioSourceComponent* clip)
		{

		}
		void XAudioBackend::Stop(Components::AudioSourceComponent* clip)
		{
		}
		void XAudioBackend::Update()
		{
		}
		XAudioSource& XAudioBackend::GetSource(Uint32 id)
		{
			if (id <= mSources.size())
			{
				return mSources[id - 1];
			}

			mSources.push_back(XAudioSource());
			return mSources.back();
		}
		XAudioBuffer& XAudioBackend::GetBuffer(Uint32 id)
		{
			if (id <= mBuffers.size())
			{
				return mBuffers[id - 1];
			}

			mBuffers.push_back(XAudioBuffer());
			return mBuffers.back();
		}
	}
}