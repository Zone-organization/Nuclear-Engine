#pragma once
#include <Assets/AssetType.h>
#include <Utilities/Hash.h>
#include <Serialization/Access.h>
#include <Utilities/UUID.h>

namespace Nuclear
{
	namespace Assets
	{
		class NEAPI IAsset
		{
		public:

			enum class State : Uint8
			{
				Unknown,              ///< Not usable asset
				Deserialized,         ///< Refers to the asset has just been deserialized (has type & name & hashedname)
				Loaded,               ///< Loaded data from disk/memory by importer
				Created               ///< Asset creation successfull also marks the data loading success
			};

			IAsset(AssetType type);

			const AssetType GetType() const;

			const State GetState() const;

			const std::string GetName();

			void SetName(const std::string& str);

			//Used for serialization
			const Utilities::UUID& GetUUID() const;

			//Used for run-time duplicate asset checking in asset library
			Uint32 GetPathHash() const;

			//TODO: Maybe remove since the client shouldnt be able to change an asset state
			void SetState(const State& state);


		protected:
			friend class AssetManager;

			//TODO: Maybe change since asset types should be compile time constant
			AssetType mType;
			State mState;

			std::string mName;
			Uint32 mPathHash = 0;
			Utilities::UUID mUUID;
		};
	}
}