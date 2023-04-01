#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Audio
	{
		class AudioBackend;

		
		struct AudioModuleDesc
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

		class NEAPI AudioModule
		{
		public:
			AudioModule(AudioModule const&) = delete;
			void operator=(AudioModule const&) = delete;

			static AudioModule& GetInstance();

			AudioBackend* GetBackend();

			bool Initialize(const AudioModuleDesc& desc);

			void Shutdown();

			void Update(/*FMOD::Channel * ch*/);
		protected:
			AudioBackend* pBackend;
		private:
			AudioModule();
		};
	}
}