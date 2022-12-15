#pragma once
#include <NE_Common.h>
#include <Utilities/Hash.h>
#include <Core/Path.h>

namespace Nuclear
{
	namespace Assets
	{
		enum class AssetType
		{
			Unknown,
			Scene,

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
			Asset(AssetType type)
			{
				mType = type;
			}

			const AssetType GetType() const
			{
				return mType;
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

			bool isValid = false;
		protected:

			AssetType mType;
			Uint32 mHashedName = 0;
			std::string mName;
			Core::Path mPath;
		};
	}
}