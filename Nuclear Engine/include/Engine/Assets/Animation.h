#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets/Common.h>
#include <Base\Math\Math.h>
#include <vector>
#include <unordered_map>
#include <Engine/Animations/BoneInfo.h>
#include <Engine/Animations/Bone.h>

namespace NuclearEngine
{
	namespace Assets
	{

		struct AnimationNodeData
		{
			Math::Matrix4 transformation;
			std::string name;
			int childrenCount;
			std::vector<AnimationNodeData> children;
		};

		class NEAPI Animation : public Asset<Animation>
		{
		public:
			Animation();
			~Animation();

			Animations::Bone* FindBone(const std::string& name);


			inline float GetTicksPerSecond() { return m_TicksPerSecond; }
			inline float GetDuration() { return m_Duration; }
			inline const AnimationNodeData& GetRootNode() { return mRootNode; }
			inline const std::unordered_map<std::string, Animations::BoneInfo>& GetBoneIDMap()
			{
				return mBoneInfoMap;
			}

		//private:

			float m_Duration;
			int m_TicksPerSecond;
			std::vector<Animations::Bone> mBones;
			AnimationNodeData mRootNode;
			std::unordered_map<std::string, Animations::BoneInfo> mBoneInfoMap;
		};

	}
}
