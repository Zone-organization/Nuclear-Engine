#pragma once
#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Audio
	{
		class AudioBackend;
		class OpenALBackend;
		class XAudioBackend;
	}
	namespace Assets
	{
		class NEAPI AudioClip : public IAsset
		{
		public:
			AudioClip();
			~AudioClip();

			const Uint32 GetBufferID() const;
		private:
			friend class Audio::XAudioBackend;
			friend class Audio::OpenALBackend;

			Uint32 mBufferID;
		};
	}
}