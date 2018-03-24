#pragma once
#include <NE_Common.h>

namespace FMOD { class System; }

namespace NuclearEngine
{
	namespace Audio
	{
		class Channel;
		class NEAPI AudioEngine
		{
		public:
			static bool Initialize(unsigned int MaxChannels = 32);

			static void Shutdown();

			static FMOD::System *GetSystem();

			static void Update(Channel * ch);
		};
	}
}