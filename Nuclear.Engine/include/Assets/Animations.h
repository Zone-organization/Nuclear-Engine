#pragma once
#include <NE_Common.h>
#include <Assets/IAsset.h>
#include <Math/Math.h>
#include <vector>
#include <unordered_map>
#include <Animation/AnimationClip.h>

namespace Nuclear
{
	namespace Assets
	{

		class NEAPI Animations : public IAsset
		{
		public:
			Animations();
			~Animations();

			std::vector<Animation::AnimationClip> mClips;
		};

	}
}
