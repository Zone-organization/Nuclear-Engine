#pragma once
#include <NE_Common.h>
#include <Utilities/Hash.h>
#include <Core/Path.h>
#include <Serialization/Access.h>

namespace Nuclear
{
	namespace Assets
	{
		enum class AssetType : Uint8
		{
			Unknown,
			SerializedScene,

			Image,
			Mesh,
			Material,
			MaterialData,
			Animations,
			AudioClip,
			Font,
			Shader,
			Script
		};

		class NEAPI Asset
		{
		public:

			enum class State : Uint8
			{
				Unknown,              ///< Not usable asset
				Deserialized,         ///< Refers to the asset has just been deserialized (has type & name & hashedname)
				Loaded,               ///< Loaded data from disk/memory by importer
				Created               ///< Asset creation successfull also marks the data loading success
			};

			Asset(AssetType type)
			{
				mType = type;
				mState = State::Unknown;
			}

			const AssetType GetType() const
			{
				return mType;
			}

			const State GetState() const
			{
				return mState;
			}

			Uint32 GetID() { return mHashedName; }
			const std::string GetName() { return mName; }
			const Core::Path GetPath() { return mPath; }

			void SetName(const std::string& str) {
				mHashedName = Utilities::Hash(str);
				mName = str;
			}

			void SetPath(const Core::Path& path) {
				mPath = path;
			}


			//TODO: Maybe remove since the client shouldnt be able to change an asset state
			void SetState(const State& state) 
			{
				mState = state;
			}

		protected:
			AssetType mType;
			State mState;

			Uint32 mHashedName = 0;
			std::string mName;
			Core::Path mPath;	
		};
	}
}