#pragma once
#include <Audio/AudioBackend.h>
#include "XAudioStructs.h"

namespace Nuclear
{
	namespace Audio
	{

		struct XAudioBuffer
		{
			AudioFile mFile;
			bool Used = false;
		};

		class NEAPI XAudioBackend : public AudioBackend
		{
		public:
			bool Initialize() override;

			void Shutdown() override;

			bool CreateAudioClip(Assets::AudioClip* result, AudioFile& file) override;

			bool CreateAudioSource(Components::AudioSourceComponent* source) override;

			void SetAudioSourceClip(Components::AudioSourceComponent* source , Assets::AudioClip* clip) override;

			void Play(Components::AudioSourceComponent* clip) override;

			void Pause(Components::AudioSourceComponent* clip) override;

			void Stop(Components::AudioSourceComponent* clip) override;

			void Update();

		protected:
			XAudioSource& GetSource(Uint32 id);
			XAudioBuffer& GetBuffer(Uint32 id);

			IXAudio2* pXAudio2;
			IXAudio2MasteringVoice* pMasterVoice;

			std::vector<XAudioSource> mSources;
			std::vector<XAudioBuffer> mBuffers;

		};
	}
}