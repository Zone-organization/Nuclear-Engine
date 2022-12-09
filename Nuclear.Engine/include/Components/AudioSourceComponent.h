#pragma once
#include <NE_Common.h>
#include <Assets\AudioClip.h>

namespace Nuclear
{
	namespace Components
	{
		class NEAPI AudioSourceComponent
		{
		public:
			AudioSourceComponent();
			~AudioSourceComponent();

			Assets::AudioClip Audio;
		};
	}
}