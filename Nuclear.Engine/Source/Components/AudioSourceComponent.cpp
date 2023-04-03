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

			Audio::AudioModule::Get().GetBackend()->SetAudioSourceClip(mSourceID, pActiveClip->GetBufferID());
		}
		void AudioSourceComponent::SetVolume(float vol)
		{
			mVolume = vol;
			Audio::AudioModule::Get().GetBackend()->SetSource_Volume(mSourceID, vol);
		}
		void AudioSourceComponent::SetPitch(float pitch)
		{
			mPitch = pitch;
			Audio::AudioModule::Get().GetBackend()->SetSource_Volume(mSourceID, pitch);
		}
		void AudioSourceComponent::SetIsLooping(bool val)
		{
			mLoop = val;
			Audio::AudioModule::Get().GetBackend()->SetSource_Volume(mSourceID, val);
		}
		void AudioSourceComponent::Play()
		{
			Audio::AudioModule::Get().GetBackend()->Play(mSourceID);
		}
		void AudioSourceComponent::Pause()
		{
			Audio::AudioModule::Get().GetBackend()->Pause(mSourceID);
		}
		void AudioSourceComponent::Stop()
		{
			Audio::AudioModule::Get().GetBackend()->Stop(mSourceID);
		}
	}
}