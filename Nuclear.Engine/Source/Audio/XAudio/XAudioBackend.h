#pragma once
#include <Audio/AudioBackend.h>

struct IXAudio2;

namespace Nuclear
{
	namespace Audio
	{

		class NEAPI XAudioBackend : public AudioBackend
		{
		public:
			bool Initialize() override;

			void Shutdown() override;

		protected:
			IXAudio2* pInstance;
		};
	}
}