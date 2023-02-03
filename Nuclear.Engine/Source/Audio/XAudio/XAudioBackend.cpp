#include "XAudioBackend.h"
#include <Utilities/Logger.h>

#include <xaudio2.h>

namespace Nuclear
{
	namespace Audio
	{
		class XAudioCallback : public IXAudio2EngineCallback
		{
			COM_DECLSPEC_NOTHROW void OnProcessingPassEnd() override
			{
			}

			COM_DECLSPEC_NOTHROW void OnProcessingPassStart() override
			{
			}

			COM_DECLSPEC_NOTHROW void OnCriticalError(HRESULT error) override
			{
				NUCLEAR_ERROR("[XAudioBackend] XAudio2 Critical Error! Code: {0}", error);
			}
		};

		bool XAudioBackend::Initialize()		
		{
			HRESULT hr = XAudio2Create(&pInstance, 0, XAUDIO2_DEFAULT_PROCESSOR);
			if (FAILED(hr))
			{
				NUCLEAR_ERROR("[XAudioBackend] Failed to initalize XAudio2! Error Code: {0}", hr);
				return false;
			}

			static XAudioCallback callbacks;

			pInstance->RegisterForCallbacks(&callbacks);

			NUCLEAR_INFO("[XAudioBackend] Initialized XAudio2 successfully.");
			return true;
		}
		void XAudioBackend::Shutdown()
		{
		}
	}
}