#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <map>
#include <vector>
#include <Engine/Assets/Animation.h>
#include <Engine/Animations/bone.h>

namespace NuclearEngine
{
	namespace Animations
	{
		class NEAPI Animator
		{
		public:
			Animator();
			Animator(Assets::Animation* animation);

			void Initialize(Assets::Animation* animation);
			
			void UpdateAnimation(float dt);

			void PlayAnimation(Assets::Animation* pAnimation);

			void CalculateBoneTransform(const Assets::AnimationNodeData* node, Math::Matrix4 parentTransform);

			std::vector<Math::Matrix4> GetFinalBoneMatrices();

		private:
			std::vector<Math::Matrix4> m_FinalBoneMatrices;
			Assets::Animation* m_CurrentAnimation;
			float m_CurrentTime;
			float m_DeltaTime;

		};
	}
}