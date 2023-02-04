#pragma once
#include <Audio/AudioBackend.h>

struct ALCdevice;
struct ALCcontext;

namespace Nuclear
{
	namespace Audio
	{

		class NEAPI OpenALBackend : public AudioBackend
		{
		public:
			OpenALBackend();
			~OpenALBackend();

			bool Initialize() override;

			void Shutdown() override;

			bool CreateAudioClip(Assets::AudioClip* result, Assets::AudioFile& file) override;

			bool CreateAudioSource(Components::AudioSourceComponent* source) override;

			void SetAudioSourceClip(Components::AudioSourceComponent* source, Assets::AudioClip* clip) override;

			void Play(Components::AudioSourceComponent* clip) override;

			void Pause(Components::AudioSourceComponent* clip) override;

			void Stop(Components::AudioSourceComponent* clip) override;

		protected:
			ALCdevice* pDevice;
			ALCcontext* pMasterContext;
		};
	}
}