#include "Engine/Components/AnimatorComponent.h"

namespace Nuclear
{
	namespace Components
	{
		AnimatorComponent::AnimatorComponent()
		{
		}
		AnimatorComponent::AnimatorComponent(Animation::Animator* animator)
		{
			mAnimator = animator;
		}
		AnimatorComponent::~AnimatorComponent()
		{
		}
	}
}