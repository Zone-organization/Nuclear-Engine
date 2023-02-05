#include "Components/AudioSourceComponent.h"
#include <Audio/AudioEngine.h>
#include <Audio/AudioBackend.h>

namespace Nuclear
{
	namespace Components
	{
		AudioSourceComponent::AudioSourceComponent()
			: pActiveClip(nullptr), mSourceID(0), mVolume(1.0f), mPitch(1.0f), mLoop(false), mState(AudioSourceComponent::State::Stopped)
		{

		}
		AudioSourceComponent::~AudioSourceComponent()
		{
		}
		void AudioSourceComponent::SetAudioClip(Assets::AudioClip* clip)
		{
			pActiveClip = clip;

			if (mSourceID == 0)
			{
				Audio::AudioEngine::GetInstance().GetBackend()->CreateAudioSource(this);
			}
			Audio::AudioEngine::GetInstance().GetBackend()->SetAudioSourceClip(this, pActiveClip);
		}
		void AudioSourceComponent::SetVolume(float vol)
		{
			mVolume = vol;
			Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Volume(this, vol);
		}
		void AudioSourceComponent::SetPitch(float pitch)
		{
			mPitch = pitch;
			Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Volume(this, pitch);
		}
		void AudioSourceComponent::SetIsLooping(bool val)
		{
			mLoop = val;
			Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Volume(this, val);
		}
		void AudioSourceComponent::Play()
		{
			Audio::AudioEngine::GetInstance().GetBackend()->Play(this);
		}
		void AudioSourceComponent::Pause()
		{
			Audio::AudioEngine::GetInstance().GetBackend()->Pause(this);
		}
		void AudioSourceComponent::Stop()
		{
			Audio::AudioEngine::GetInstance().GetBackend()->Stop(this);
		}
	}
}