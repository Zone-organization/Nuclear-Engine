#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine\Assets\AudioClip.h>

namespace NuclearEngine
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