#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <vector>

namespace NuclearEngine {
	namespace Animation
	{
		class Animator;
	}
	namespace Components
	{

		class NEAPI AnimatorComponent
		{
		public:
			AnimatorComponent();
			AnimatorComponent(Animation::Animator* animator);
			~AnimatorComponent();


			Animation::Animator* mAnimator = nullptr;
		};

	}
}