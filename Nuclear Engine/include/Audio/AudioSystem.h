#pragma once
#include <NE_Common.h>

namespace FMOD { class System; }

namespace NuclearEngine
{
	namespace Audio
	{
		class Channel;
		class NEAPI AudioSystem
		{
		public:
			static bool Initialize(unsigned int MaxChannels = 32);

			static FMOD::System *GetSystem();

			static void Update(Channel * ch);
		};
	}
}