#pragma once
#include <NE_Common.h>

namespace FMOD
{
	class System;
	class Channel;
}

namespace Nuclear
{
	namespace Audio
	{
		struct AudioEngineDesc
		{
			unsigned int MaxChannels = 32;
		};

		class NEAPI AudioEngine
		{
		public:
			AudioEngine(AudioEngine const&) = delete;
			void operator=(AudioEngine const&) = delete;

			static AudioEngine& GetInstance();

			bool Initialize(const AudioEngineDesc& desc);

			void Shutdown();

			FMOD::System *GetSystem();

			void Update(FMOD::Channel * ch);
		protected:
			FMOD::System* pSystem;

		private:
			AudioEngine();
		};
	}
}