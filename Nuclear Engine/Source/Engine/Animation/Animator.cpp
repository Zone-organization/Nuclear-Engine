#include "Engine/Animations/Animator.h"

namespace NuclearEngine
{
	namespace Animations
	{
		Animator::Animator()
		{
		}
		inline Animator::Animator(Assets::Animation* animation)
		{
			Initialize(animation);
		}
		void Animator::Initialize(Assets::Animation* animation)
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
		inline void Animator::PlayAnimation(Assets::Animation* pAnimation)
		{
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
		}
		inline void Animator::CalculateBoneTransform(const Assets::AnimationNodeData* node, Math::Matrix4 parentTransform)
		{
			std::string nodeName = node->name;
			Math::Matrix4 nodeTransform = node->transformation;

			Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

			if (Bone)
			{
				Bone->Update(m_CurrentTime);
				nodeTransform = Bone->GetLocalTransform();
			}

			Math::Matrix4 globalTransformation = parentTransform * nodeTransform;

			auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
			if (boneInfoMap.find(nodeName) != boneInfoMap.end())
			{
				int index = boneInfoMap[nodeName].id;
				Math::Matrix4 offset = boneInfoMap[nodeName].offset;
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