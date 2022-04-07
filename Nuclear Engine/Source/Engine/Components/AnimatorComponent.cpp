#include "Engine/Components/AnimatorComponent.h"

namespace NuclearEngine
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