#pragma once
#include <NE_Common.h>
#include <Assets/ImportingDescs.h>
#include <Assets/Model.h>
#include <Assets/TextureDesc.h>
#include <Assets\Importers\AssimpImporter.h>
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
		class Script;
		class AssetLibrary;
		class Texture;

		class NEAPI Importer
		{
		public:
			static Importer& GetInstance();
			Importer(const Importer&) = delete;
			Importer& operator= (const Importer) = delete;

			void Test();

			Texture* ImportTexture(const Core::Path& Path, const TextureImportingDesc& Desc = TextureImportingDesc());
			Texture* ImportTexture(const TextureDesc& Imagedata, const TextureImportingDesc& Desc = TextureImportingDesc());

			//	Model* AsyncImportModel(const Core::Path& Path,  const ModelImportingDesc& desc = ModelImportingDesc());

			Texture* ImportTextureST(const Core::Path& Path, const TextureImportingDesc& Desc = TextureImportingDesc());
			//	Image* ImportTexture(const TextureDesc& Imagedata,  const TextureImportingDesc& Desc = TextureImportingDesc());

			AudioClip* ImportAudioClip(const Core::Path& Path, const AudioClipImportingDesc& Desc = AudioClipImportingDesc());

			//TODO: Separate to ImportMesh & MaterialData & Animations
			Model* ImportModel(const Core::Path& Path, const ModelImportingDesc& desc = ModelImportingDesc());

			Font* ImportFont(const Core::Path& Path, const FontImportingDesc& desc = FontImportingDesc());

			Shader* ImportShader(const Core::Path& Path, const ShaderImportingDesc& desc = ShaderImportingDesc());

			Script* ImportScript(const Core::Path& Path, const ScriptImportingDesc& desc = ScriptImportingDesc());

			Scene* ImportScene(const Core::Path& Path, const SceneImportingDesc& desc = SceneImportingDesc());

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<Texture*, 6> ImportTextureCube(const std::array<Core::Path, 6 >& Paths, const TextureImportingDesc& Desc);

			AssetType GetAssetType(const std::string& filename);

			std::vector<IAsset*>& GetQueuedAssets();

		private:
			Texture* TextureCube_Import(const Core::Path& Path, const TextureImportingDesc& Desc);
			Importers::AssimpImporter mAssimpImporter;
			msdfgen::FreetypeHandle* FT_Handle;

			std::vector<IAsset*> mQueuedAssets;

			//std::vector<Threading::Task*> mQueuedTasks;
			Importer();
		};
	}
}