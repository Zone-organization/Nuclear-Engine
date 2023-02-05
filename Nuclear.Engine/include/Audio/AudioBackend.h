#pragma once
#include <Audio/AudioFile.h>

namespace Nuclear
{
	namespace Components
	{
		class AudioSourceComponent;
	}

	namespace Assets
	{
		class AudioClip;
	}
	namespace Audio
	{
		

		class NEAPI AudioBackend
		{
		public:
			virtual bool Initialize() = 0;

			virtual void Shutdown() = 0;

			virtual bool CreateAudioClip(Assets::AudioClip* result, AudioFile& file) = 0;

			virtual bool CreateAudioSource(Components::AudioSourceComponent* source) = 0;;

			virtual void SetAudioSourceClip(Components::AudioSourceComponent* source, Assets::AudioClip* clip) = 0;;

			virtual void Play(Components::AudioSourceComponent* audio_source) = 0;

			virtual void Pause(Components::AudioSourceComponent* audio_source) = 0;

			virtual void Stop(Components::AudioSourceComponent* audio_source) = 0;

			virtual void SetSource_Volume(Components::AudioSourceComponent* audio_source, float vol) = 0;
			virtual void SetSource_Pitch(Components::AudioSourceComponent* audio_source, float pitch) = 0;
			virtual void SetSource_IsLooping(Components::AudioSourceComponent* audio_source, bool val) = 0;
			
		};
	}
}