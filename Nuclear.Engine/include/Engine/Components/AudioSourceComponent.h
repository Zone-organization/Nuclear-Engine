#pragma once
#include <Core\NE_Common.h>
#include <Engine\Assets\AudioClip.h>

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