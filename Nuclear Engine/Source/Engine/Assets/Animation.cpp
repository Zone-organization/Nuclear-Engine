#include "Engine/Assets/Animation.h"

namespace NuclearEngine
{
	namespace Assets
	{
		Animation::Animation()
		{
		}
		Animation::~Animation()
		{
		}
		inline Animations::Bone* Animation::FindBone(const std::string& name)
		{
			auto iter = std::find_if(mBones.begin(), mBones.end(),
				[&](const Animations::Bone& Bone)
				{
					return Bone.GetBoneName() == name;
				}
			);
			if (iter == mBones.end()) return nullptr;
			else return &(*iter);
		}
	}
}