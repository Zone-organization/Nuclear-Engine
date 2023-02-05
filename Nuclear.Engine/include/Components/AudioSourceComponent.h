#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Audio { class AudioBackend; }
	namespace Assets { class AudioClip; }

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

			Uint32 mSourceID;					//Openal = source id , XAudio index in sources vector
		};
	}
}