#pragma once
#include <Base\NE_Common.h>

namespace FMOD
{
	class System;
	class Channel;
}

namespace NuclearEngine
{
	namespace Audio
	{
		class NEAPI AudioEngine
		{
		public:
			static bool Initialize(unsigned int MaxChannels = 32);

			static void Shutdown();

			static FMOD::System *GetSystem();

			static void Update(FMOD::Channel * ch);
		};
	}
}