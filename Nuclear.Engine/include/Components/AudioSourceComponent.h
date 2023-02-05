#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Audio { 
		class AudioBackend; 
		class OpenALBackend;
		class XAudioBackend;
	}
	namespace Assets { class AudioClip; }

	namespace Components
	{
		class NEAPI AudioSourceComponent
		{
		public:
			enum class State
			{
				Stopped = 0,
				Playing = 1,
				Paused = 2
			};

			AudioSourceComponent();
			~AudioSourceComponent();

			FORCE_INLINE Assets::AudioClip* GetAudioClip()
			{
				return pActiveClip;
			}
			void SetAudioClip(Assets::AudioClip* clip);

			FORCE_INLINE Uint32 GetSourceID() const
			{
				return mSourceID;
			}

			void SetVolume(float vol);
			void SetPitch(float pitch);
			void SetIsLooping(bool val);

			FORCE_INLINE float GetVolume() const
			{
				return mVolume;
			}
			FORCE_INLINE float GetPitch() const
			{
				return mPitch;
			}
			FORCE_INLINE bool  GetLoop() const
			{
				return mLoop;
			}

			FORCE_INLINE State GetState() const
			{
				return mState;
			}

			void Play();
			void Pause();
			void Stop();
		protected:
			friend class Audio::AudioBackend;
			friend class Audio::XAudioBackend;
			friend class Audio::OpenALBackend;


			State mState;
			Uint32 mSourceID;					//Openal = source id , XAudio index in sources vector

		private:
			float mVolume;   //Note: [0, 1] range
			float mPitch;
			bool mLoop;
			Assets::AudioClip* pActiveClip;
		};
	}
}