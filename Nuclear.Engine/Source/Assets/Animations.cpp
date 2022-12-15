#include "Assets/Animations.h"

namespace Nuclear
{
	namespace Assets
	{
		Animations::Animations()
			: Asset(AssetType::Animations)
		{
		}
		Animations::~Animations()
		{
			mClips.clear();
		}
	}
}