#pragma once
#include <Core\NE_Common.h>

namespace Nuclear {
	namespace Animation
	{
		class Animator;
	}
	namespace Components
	{

		//TODO: Rename to SkinnedMeshComponent
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