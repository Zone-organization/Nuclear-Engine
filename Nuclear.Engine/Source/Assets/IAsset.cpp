#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Assets
	{
		IAsset::IAsset(AssetType type)
		{
			mType = type;
			mState = State::Unknown;
		}

		const AssetType IAsset::GetType() const
		{
			return mType;
		}

		const IAsset::State IAsset::GetState() const
		{
			return mState;
		}

		Uint32 IAsset::GetPathHash() const
		{
			return mPathHash;
		}
		const std::string IAsset::GetName()
		{ 
			return mName; 
		}

		void IAsset::SetName(const std::string& str) {
			mName = str;
		}

		const Utilities::UUID& IAsset::GetUUID() const
		{
			return mUUID;
		}

		void IAsset::SetState(const State& state)
		{
			mState = state;
		}

	}
}