#include "Assets/Animations.h"

namespace Nuclear
{
	namespace Assets
	{
		Animations::Animations()
			: IAsset(AssetType::Animations)
		{
		}
		Animations::~Animations()
		{
			mClips.clear();
		}
	}
}