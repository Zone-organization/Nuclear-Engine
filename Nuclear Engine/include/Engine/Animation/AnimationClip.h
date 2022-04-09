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
			~ClipNodeData()
			{
				childrenCount = 0;
				name = std::string();
				children.clear();
			}
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


			inline Float64 GetTicksPerSecond() { return m_TicksPerSecond; }
			inline Float64 GetDuration() { return m_Duration; }

			inline const ClipNodeData& GetRootNode() { return mRootNode; }
			inline const std::unordered_map<std::string, Animation::BoneInfo>& GetBoneIDMap()
			{
				return mBoneInfoMap;
			}

			//private:

			Float64 m_Duration;
			Float64 m_TicksPerSecond;
			std::vector<Animation::Bone> mBones;
			ClipNodeData mRootNode;
			std::unordered_map<std::string, Animation::BoneInfo> mBoneInfoMap;

		};
	}
}