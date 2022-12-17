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

		template<class T>
		class NEAPI AssetPtr
		{
		public:
			AssetPtr()
			{

			}

			AssetType mType;
			T* pAsset;
		};
	}
}