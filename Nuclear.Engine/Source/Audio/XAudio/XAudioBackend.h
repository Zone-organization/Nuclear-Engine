#pragma once
#include <Audio/AudioBackend.h>

struct IXAudio2;
struct IXAudio2MasteringVoice;
namespace Nuclear
{
	namespace Audio
	{

		class NEAPI XAudioBackend : public AudioBackend
		{
		public:
			bool Initialize() override;

			void Shutdown() override;

			bool CreateAudioClip(Assets::AudioClip* result, Assets::AudioFile& file) override;

			bool CreateAudioSource(Components::AudioSourceComponent* source) override;

			void SetAudioSourceClip(Components::AudioSourceComponent* source , Assets::AudioClip* clip) override;

			void Play(Components::AudioSourceComponent* clip) override;

			void Pause(Components::AudioSourceComponent* clip) override;

			void Stop(Components::AudioSourceComponent* clip) override;

		protected:
			IXAudio2* pXAudio2;
			IXAudio2MasteringVoice* pMasterVoice;
		};
	}
}