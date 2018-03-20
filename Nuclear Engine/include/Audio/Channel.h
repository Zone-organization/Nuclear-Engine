#pragma once
#include <NE_Common.h>
namespace FMOD { class Channel; }

namespace NuclearEngine
{
	namespace Audio
	{
		class NEAPI Channel
		{
		public:

			FMOD::Channel **GetChannel();
		private:
			FMOD::Channel *channel;
		};
	}
}