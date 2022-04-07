#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <vector>

namespace NuclearEngine {
	namespace Animation
	{
		class Animator;
	}
	namespace Components
	{

		class NEAPI AnimatorComponent : public ECS::Component<AnimatorComponent>
		{
		public:
			AnimatorComponent();
			AnimatorComponent(Animation::Animator* animator);
			~AnimatorComponent();


			Animation::Animator* mAnimator = nullptr;
		};

	}
}