#include "Engine/Animation/AnimationClip.h"

namespace NuclearEngine
{
	namespace Animation
	{
		AnimationClip::AnimationClip()
		{
		}
		AnimationClip::~AnimationClip()
		{
			m_Duration = 0;
			m_TicksPerSecond = 0;
			mBones.clear();
			mBoneInfoMap.clear();
		}

		inline Animation::Bone* AnimationClip::FindBone(const std::string& name)
		{
			auto iter = std::find_if(mBones.begin(), mBones.end(),
				[&](const Animation::Bone& Bone)
				{
					return Bone.GetBoneName() == name;
				}
			);
			if (iter == mBones.end()) return nullptr;
			else return &(*iter);
		}
	}
}