#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include <Compiler.h>

namespace Nuclear
{
	namespace Audio
	{
		struct XAudioSource
		{
			IXAudio2SourceVoice* pVoice = nullptr;
			X3DAUDIO_EMITTER mData;
			WAVEFORMATEX mFormat;

			FORCE_INLINE bool Unused() const
			{
				return (pVoice == nullptr);
			}
		};
	}
}