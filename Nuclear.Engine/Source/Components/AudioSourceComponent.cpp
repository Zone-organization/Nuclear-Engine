#include "Components/AudioSourceComponent.h"
#include <Audio/AudioModule.h>
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
			: pActiveClip(clip), mSourceID(0), mVolume(1.0f), mPitch(1.0f), mLoop(false), mState(AudioSourceComponent::State::Stopped), mPrevPos(0.0f), mVelocity(0.0f)
		{
		}
		AudioSourceComponent::~AudioSourceComponent()
		{
		}
		void AudioSourceComponent::SetAudioClip(Assets::AudioClip* clip)
		{
			pActiveClip = clip;

			Audio::AudioModule::GetInstance().GetBackend()->SetAudioSourceClip(mSourceID, pActiveClip->GetBufferID());
		}
		void AudioSourceComponent::SetVolume(float vol)
		{
			mVolume = vol;
			Audio::AudioModule::GetInstance().GetBackend()->SetSource_Volume(mSourceID, vol);
		}
		void AudioSourceComponent::SetPitch(float pitch)
		{
			mPitch = pitch;
			Audio::AudioModule::GetInstance().GetBackend()->SetSource_Volume(mSourceID, pitch);
		}
		void AudioSourceComponent::SetIsLooping(bool val)
		{
			mLoop = val;
			Audio::AudioModule::GetInstance().GetBackend()->SetSource_Volume(mSourceID, val);
		}
		void AudioSourceComponent::Play()
		{
			Audio::AudioModule::GetInstance().GetBackend()->Play(mSourceID);
		}
		void AudioSourceComponent::Pause()
		{
			Audio::AudioModule::GetInstance().GetBackend()->Pause(mSourceID);
		}
		void AudioSourceComponent::Stop()
		{
			Audio::AudioModule::GetInstance().GetBackend()->Stop(mSourceID);
		}
	}
}