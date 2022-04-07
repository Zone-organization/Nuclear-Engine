#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <vector>
#include <unordered_map>
#include <Engine/Animation/BoneInfo.h>
#include <Engine/Animation/Bone.h>

namespace NuclearEngine
{
	namespace Animation
	{
		struct ClipNodeData
		{
			Math::Matrix4 transformation;
			std::string name;
			int childrenCount;
			std::vector<ClipNodeData> children;
		};
		class NEAPI AnimationClip
		{
		public:
			AnimationClip();
			~AnimationClip();

			Animation::Bone* FindBone(const std::string& name);


			inline float GetTicksPerSecond() { return m_TicksPerSecond; }
			inline float GetDuration() { return m_Duration; }
			inline const ClipNodeData& GetRootNode() { return mRootNode; }
			inline const std::unordered_map<std::string, Animation::BoneInfo>& GetBoneIDMap()
			{
				return mBoneInfoMap;
			}

			//private:

			float m_Duration;
			int m_TicksPerSecond;
			std::vector<Animation::Bone> mBones;
			ClipNodeData mRootNode;
			std::unordered_map<std::string, Animation::BoneInfo> mBoneInfoMap;

		};
	}
}