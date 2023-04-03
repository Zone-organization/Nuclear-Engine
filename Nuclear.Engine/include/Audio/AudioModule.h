#pragma once
#include <NE_Common.h>
#include <Core/EngineModule.h>

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

		class NEAPI AudioModule : public Core::EngineModule<AudioModule>
		{
			friend class Core::EngineModule<AudioModule>;
		public:
			AudioBackend* GetBackend();

			bool Initialize(const AudioModuleDesc& desc);

			void Shutdown() override;

			void Update(/*FMOD::Channel * ch*/);

		protected:
			AudioBackend* pBackend;

			AudioModule();
		};
	}
}