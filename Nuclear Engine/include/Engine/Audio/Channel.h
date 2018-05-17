#pragma once
#include <Base\NE_Common.h>
namespace FMOD { class Channel; }

namespace NuclearEngine
{
	namespace Audio
	{
		class NEAPI Channel
		{
		public:
			FMOD::Channel *GetChannel();

			FMOD::Channel **GetChannelPtr();
		private:
			FMOD::Channel *channel;
		};
	}
}