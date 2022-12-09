#include "Animation/Animator.h"

namespace Nuclear
{
	namespace Animation
	{
		Animator::Animator()
		{
			m_CurrentAnimation = nullptr;
			m_CurrentTime = 0;
			m_DeltaTime = 0;
		}
		inline Animator::Animator(Animation::AnimationClip* animation)
		{
			Initialize(animation);
		}
		void Animator::Initialize(Animation::AnimationClip* animation)
		{
			m_CurrentTime = 0.0;
			m_CurrentAnimation = animation;

			m_FinalBoneMatrices.reserve(100);

			for (int i = 0; i < 100; i++)
				m_FinalBoneMatrices.push_back(Math::Matrix4(1.0f));
		}
		inline void Animator::UpdateAnimation(float dt)
		{
			m_DeltaTime = dt;
			if (m_CurrentAnimation)
			{
				m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
				m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
				CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), Math::Matrix4(1.0f));
			}
		}
		inline void Animator::PlayAnimation(Animation::AnimationClip* pAnimation)
		{
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
		}
		inline void Animator::CalculateBoneTransform(const Animation::ClipNodeData* node, Math::Matrix4 parentTransform)
		{
			Uint32 nodehash = node->mHashedName;
			Math::Matrix4 nodeTransform = node->transformation;

			Bone* Bone = m_CurrentAnimation->FindBone(nodehash);

			if (Bone)
			{
				Bone->Update(m_CurrentTime);
				nodeTransform = Bone->GetLocalTransform();
			}

			Math::Matrix4 globalTransformation = parentTransform * nodeTransform;

			auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
			if (boneInfoMap.find(nodehash) != boneInfoMap.end())
			{
				int index = boneInfoMap[nodehash].id;
				Math::Matrix4 offset = boneInfoMap[nodehash].offset;
				m_FinalBoneMatrices[index] = globalTransformation * offset;
			}

			for (int i = 0; i < node->childrenCount; i++)
				CalculateBoneTransform(&node->children[i], globalTransformation);
		}
		inline std::vector<Math::Matrix4> Animator::GetFinalBoneMatrices()
		{
			return m_FinalBoneMatrices;
		}
	}
}