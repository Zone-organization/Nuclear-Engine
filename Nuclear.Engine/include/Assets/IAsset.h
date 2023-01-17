#pragma once
#include <Assets/AssetType.h>
#include <Utilities/Hash.h>
#include <Serialization/Access.h>
#include <Core/UUID.h>

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
				Deserialized,         ///< Refers to the asset has just been deserialized (has type & name & hashedname) (not queued or loaded)
				Queued,			      ///< Queued for loading
				Loaded,               ///< Loaded data from disk/memory by importer
				Created               ///< Asset creation successfull also marks the data loading success
			};

			IAsset(AssetType type);
			virtual ~IAsset();

			const AssetType GetType() const;

			const State GetState() const;

			const std::string GetName();

			void SetName(const std::string& str);

			//Used for serialization
			const Core::UUID& GetUUID() const;

			//TODO: Maybe remove since the client shouldnt be able to change an asset state
			void SetState(const State& state);

			void SetUUID(const Core::UUID& uuid);
		protected:
			friend class AssetManager;
			friend class Importer;
			friend Serialization::Access;

			//TODO: Maybe change since asset types should be compile time constant
			AssetType mType;
			State mState;

			std::string mName;
			Core::UUID mUUID;
		};
	}
}