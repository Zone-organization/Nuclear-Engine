#pragma once
#include <NE_Common.h>
#include <Assets/LoadingDescs.h>
#include <Assets/TextureDesc.h>
#include <Core/Path.h>

typedef struct FT_LibraryRec_* FT_Library;
namespace msdfgen { class FreetypeHandle; }
namespace Nuclear
{
	namespace Assets
	{
		class IAsset;
		class AudioClip;
		class Font;
		class Shader;
		class Scene;
		class Mesh;
		class Script;
		class AssetLibrary;
		class Texture;
		class Material;

		class NEAPI Loader
		{
		public:
			static Loader& Get();
			Loader(const Loader&) = delete;
			Loader& operator= (const Loader) = delete;

			Texture* LoadTexture(const Core::Path& Path,const Assets::AssetMetadata& metadata);
			Material* LoadMaterial(const Core::Path& Path, const Assets::AssetMetadata& metadata);
			Shader* LoadShader(const Core::Path& Path, const Assets::AssetMetadata& metadata);
			Mesh* LoadMesh(const Core::Path& Path,bool auto_load_external_material , const Assets::AssetMetadata& metadata);

			std::vector<IAsset*>& GetQueuedAssets();

		private:
			std::vector<IAsset*> mQueuedAssets;

			Loader();
		};
	}
}