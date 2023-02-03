#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Audio
	{
		class AudioBackend;

		
		struct AudioEngineDesc
		{
			enum class AudioBackendType : Uint8
			{
				Unknown,
				OpenAL = 1,
				XAudio2 = 2,

				MAX_UNKNOWN = UINT8_MAX
			};

			AudioBackendType mRequestedBackend = AudioBackendType::Unknown;
			unsigned int MaxChannels = 32;
		};

		class NEAPI AudioEngine
		{
		public:
			AudioEngine(AudioEngine const&) = delete;
			void operator=(AudioEngine const&) = delete;

			static AudioEngine& GetInstance();

			AudioBackend* GetBackend();

			bool Initialize(const AudioEngineDesc& desc);

			void Shutdown();

			void Update(/*FMOD::Channel * ch*/);
		protected:
			AudioBackend* pBackend;
		private:
			AudioEngine();
		};
	}
}