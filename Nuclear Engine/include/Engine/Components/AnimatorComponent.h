#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <vector>

namespace NuclearEngine {
	namespace Animations
	{
		class Animator;
	}
	namespace Components
	{

		class NEAPI AnimatorComponent : public ECS::Component<AnimatorComponent>
		{
		public:
			AnimatorComponent();
			AnimatorComponent(Animations::Animator* animator);
			~AnimatorComponent();


			Animations::Animator* mAnimator = nullptr;
		};

	}
}