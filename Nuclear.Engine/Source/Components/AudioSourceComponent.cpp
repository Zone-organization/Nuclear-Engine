#include "Components/AudioSourceComponent.h"
#include <Audio/AudioEngine.h>
#include <Audio/AudioBackend.h>
#include <Assets/AudioClip.h>
namespace Nuclear
{
	namespace Components
	{
		AudioSourceComponent::AudioSourceComponent()
			: AudioSourceComponent(nullptr)
		{

		}
		AudioSourceComponent::AudioSourceComponent(Assets::AudioClip* clip)
			: pActiveClip(clip), mSourceID(0), mVolume(1.0f), mPitch(1.0f), mLoop(false), mState(AudioSourceComponent::State::Stopped)
		{
		}
		AudioSourceComponent::~AudioSourceComponent()
		{
		}
		void AudioSourceComponent::SetAudioClip(Assets::AudioClip* clip)
		{
			pActiveClip = clip;

			Audio::AudioEngine::GetInstance().GetBackend()->SetAudioSourceClip(mSourceID, pActiveClip->GetBufferID());
		}
		void AudioSourceComponent::SetVolume(float vol)
		{
			mVolume = vol;
			Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Volume(mSourceID, vol);
		}
		void AudioSourceComponent::SetPitch(float pitch)
		{
			mPitch = pitch;
			Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Volume(mSourceID, pitch);
		}
		void AudioSourceComponent::SetIsLooping(bool val)
		{
			mLoop = val;
			Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Volume(mSourceID, val);
		}
		void AudioSourceComponent::Play()
		{
			Audio::AudioEngine::GetInstance().GetBackend()->Play(mSourceID);
		}
		void AudioSourceComponent::Pause()
		{
			Audio::AudioEngine::GetInstance().GetBackend()->Pause(mSourceID);
		}
		void AudioSourceComponent::Stop()
		{
			Audio::AudioEngine::GetInstance().GetBackend()->Stop(mSourceID);
		}
	}
}