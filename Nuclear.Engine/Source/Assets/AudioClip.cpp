#include <Assets\AudioClip.h>

namespace Nuclear
{
	namespace Assets
	{
		AudioClip::AudioClip()
			: IAsset(AssetType::AudioClip), mBufferID(0)
		{

		}
		AudioClip::~AudioClip()
		{

		}
		const Uint32 AudioClip::GetBufferID() const
		{
			return mBufferID;
		}

	}
}