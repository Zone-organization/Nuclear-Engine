#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine\Assets\AudioClip.h>

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI AudioSourceComponent : public ECS::Component<AudioSourceComponent>
		{
		public:
			AudioSourceComponent();
			~AudioSourceComponent();

			Assets::AudioClip Audio;
		};
	}
}