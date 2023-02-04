#pragma once
#include <NE_Common.h>
#include <Assets\AudioClip.h>

struct IXAudio2SourceVoice;
namespace Nuclear
{
	//namespace Audio { class AudioBackend; }

	namespace Components
	{
		class NEAPI AudioSourceComponent
		{
		public:
			AudioSourceComponent();
			~AudioSourceComponent();

			
		//protected:
		//	friend class Audio::AudioBackend;
			Assets::AudioClip* pActiveClip;

			Uint32 mSourceID;					//Openal
			IXAudio2SourceVoice* pSourceVoice;  //XAudio2
		};
	}
}