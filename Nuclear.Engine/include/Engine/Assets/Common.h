#pragma once
#include <Core\NE_Common.h>
#include <Core\Utilities\Hash.h>
#include <Core\Path.h>
#include <cereal/access.hpp>

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
			Shader
		};
		template <class T>
		class Asset
		{
		public:
			const char* GetType()
			{
				return typeid(T).name();
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
			friend class cereal::access;

			Uint32 mHashedName = 0;
			std::string mName;
			Core::Path mPath;
		};
	}
}