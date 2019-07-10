#pragma once
#include <Base\NE_Common.h>
#include <FMOD/inc/fmod.hpp>

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