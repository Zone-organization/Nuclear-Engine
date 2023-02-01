#pragma once
#include <NE_Common.h>
#include <Assets/AssetMetadata.h>

namespace Nuclear
{
	namespace Assets
	{
		class Shader;
		class Scene;
		class Mesh;
		class Texture;
		class Material;
	}
	namespace Serialization
	{
		
	/*	template<class T>
		Assets::AssetType GetAssetType()
		{
			return Assets::AssetType::Unknown;
		}*/

		class NEAPI SerializationEngine
		{
		public:
			static SerializationEngine& GetInstance();

			SerializationEngine(const SerializationEngine&) = delete;
			SerializationEngine& operator= (const SerializationEngine) = delete;

			bool Serialize(const Assets::AssetMetadata& metadata, const Core::Path& path);

			bool Deserialize(Assets::AssetMetadata& inmetadata, const Core::Path& path);


			//template<class T>
			//T* DeserializeAsset(const Core::UUID& uuid)
			//{
			//	static_assert(std::is_base_of<Assets::IAsset, T>::value, "DeserializeAsset<T> class must derive from IAsset!");

			//	return static_cast<T*>(DeserializeUUID(GetAssetType<T>(), uuid));
			//}


			Assets::IAsset* DeserializeUUID(Assets::AssetType type, const Core::UUID& uuid);

		private:
			SerializationEngine();
		};


		//template <> Assets::AssetType GetAssetType<Assets::Mesh>() { return Assets::AssetType::Mesh; }
		//template <> Assets::AssetType GetAssetType<Assets::Material>() { return Assets::AssetType::Material; }
		//template <> Assets::AssetType GetAssetType<Assets::Texture>() { return Assets::AssetType::Texture; }
		//template <> Assets::AssetType GetAssetType<Assets::Shader>() { return Assets::AssetType::Shader; }
		//template <> Assets::AssetType GetAssetType<Assets::Scene>() { return Assets::AssetType::Scene; }

	}
}