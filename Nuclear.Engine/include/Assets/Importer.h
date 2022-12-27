#pragma once
#include <NE_Common.h>
#include <Assets/ImportingDescs.h>
#include <Assets/Model.h>
#include <Assets\Importers\AssimpImporter.h>
#include <Assets\Importers\FreeImageImporter.h>
#include <Graphics/Texture.h>
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
		struct AssetLibrary;



		class NEAPI Importer
		{
		public:
			static Importer& GetInstance();
			Importer(const Importer&) = delete;
			Importer& operator= (const Importer) = delete;

			void Test();

			Image* ImportImage(const Core::Path& Path, AssetLibrary* library, const ImageImportingDesc& Desc = ImageImportingDesc());
			Image* ImportImage(const ImageData& Imagedata, AssetLibrary* library, const ImageImportingDesc& Desc = ImageImportingDesc());

			Graphics::Texture ImportTexture(const Core::Path& Path, AssetLibrary* library, const TextureImportingDesc& Desc = TextureImportingDesc());
			Graphics::Texture ImportTexture(const ImageData& Imagedata, AssetLibrary* library, const TextureImportingDesc& Desc = TextureImportingDesc());

			AudioClip* ImportAudioClip(const Core::Path& Path, AssetLibrary* library, const AudioClipImportingDesc& Desc = AudioClipImportingDesc());

			//TODO: Separate to ImportMesh & MaterialData & Animations
			Model* ImportModel(const Core::Path& Path, AssetLibrary* library, const ModelImportingDesc& desc = ModelImportingDesc());

			Font* ImportFont(const Core::Path& Path, AssetLibrary* library, const FontImportingDesc& desc = FontImportingDesc());

			Shader* ImportShader(const Core::Path& Path, AssetLibrary* library, const ShaderImportingDesc& desc = ShaderImportingDesc());

			Script* ImportScript(const Core::Path& Path, AssetLibrary* library, const ScriptImportingDesc& desc = ScriptImportingDesc());

			Scene* ImportScene(const Core::Path& Path, AssetLibrary* library, const SceneImportingDesc& desc = SceneImportingDesc());

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<Image*, 6> ImportTextureCube(const std::array<Core::Path, 6 >& Paths, AssetLibrary* library, const ImageImportingDesc& Desc);

			AssetType GetAssetType(const std::string& filename);
		private:
			Image* TextureCube_Import(const Core::Path& Path, AssetLibrary* library, const ImageImportingDesc& Desc);
			void FinishImportingAsset(IAsset* asset, const Core::Path& path, Uint32 Hashedpath, const std::string& libraryname, bool log = true);
			Importers::AssimpImporter mAssimpImporter;
			msdfgen::FreetypeHandle* FT_Handle;

			std::vector<Importers::FreeImageImporter> mImageTasks;

			Importer();
		};
	}
}