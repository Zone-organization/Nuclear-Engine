#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <map>
#include <vector>
//#include <Assets/Animation.h>
#include <Animation/bone.h>
#include <Animation/AnimationClip.h>

namespace Nuclear
{
	namespace Assets
	{
		class Animations;
	}
	namespace Animation
	{
		class NEAPI Animator
		{
		public:
			Animator();
			Animator(Animation::AnimationClip* animation);

			void Initialize(Assets::Animations* animation);

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