#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <map>
#include <vector>
//#include <Engine/Assets/Animation.h>
#include <Engine/Animation/bone.h>
#include <Engine/Animation/AnimationClip.h>

namespace Nuclear
{
	namespace Animation
	{
		class NEAPI Animator
		{
		public:
			Animator();
			Animator(Animation::AnimationClip* animation);

			void Initialize(Animation::AnimationClip* animation);
			
			void UpdateAnimation(float dt);

			void PlayAnimation(Animation::AnimationClip* pAnimation);

			void CalculateBoneTransform(const Animation::ClipNodeData* node, Math::Matrix4 parentTransform);

			std::vector<Math::Matrix4> GetFinalBoneMatrices();

		private:
			std::vector<Math::Matrix4> m_FinalBoneMatrices;
			Animation::AnimationClip* m_CurrentAnimation;
			float m_CurrentTime;
			float m_DeltaTime;

		};
	}
}