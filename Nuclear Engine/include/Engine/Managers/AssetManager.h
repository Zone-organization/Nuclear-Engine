#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Assets/Texture.h>
#include <Base\Utilities\Delegate.h>
#include <unordered_map>
#include <array>
#include <string>
namespace NuclearEngine {
	namespace Assets
	{
		class Mesh;
		class Material;
	}
	namespace Managers
	{

		struct MeshLoadingDesc
		{
			bool UseTexCoords = true;
			bool UseNormals = true;
			bool UseTangents = false;
			bool LoadDiffuseTextures = true;
			bool LoadSpecularTextures = false;
			bool LoadNormalTextures = false;
		};

		typedef Utilities::Delegate<Assets::Mesh&(const std::string& Path, const Managers::MeshLoadingDesc& desc)> MeshImport;
		typedef Utilities::Delegate<LLGL::Texture_Data(const std::string& Path, const LLGL::Texture_Desc & Desc)> TextureImport;

		class NEAPI AssetManager {
		public:
			//Public Members
			//All loaded textures with their hashed names with crc32c (always saved)
			static std::unordered_map<Uint32, Assets::Texture> mImportedTextures;
			//Real pre-hashed texture names with paths (conditionally saved see SaveTextureNames)
			static std::unordered_map<Uint32,std::string> mHashedTexturesNames;
			//tells the asset manager whether to store the real texture name or not
			static bool mSaveTextureNames;
			
			//All imported meshes with their hashed names with crc32c (always saved)
			static std::unordered_map<Uint32, Assets::Mesh> mImportedMeshes;
			//Real pre-hashed meshes names with paths (conditionally saved see SaveTextureNames)
			static std::unordered_map<Uint32, std::string> mHashedMeshesNames;
			//tells the asset manager whether to store the real mesh name or not
			static bool mSaveMeshNames;

			//All imported materials with their hashed names with crc32c (always saved)
			static std::unordered_map<Uint32, Assets::Material> mImportedMaterials;
			//Real pre-hashed meshes names with paths (conditionally saved see SaveTextureNames)
			static std::unordered_map<Uint32, std::string> mHashedMaterialsNames;
			//tells the asset manager whether to store the real mesh name or not
			static bool mSaveMaterialsNames;

			//Methods
			static void Initialize(bool SaveTextureNames = false);
			static void ShutDown();
			
			static Assets::Mesh& Import(const std::string& Path, const MeshLoadingDesc& desc, Assets::Material& material);
			static Assets::Texture& Import(const std::string& Path, const LLGL::Texture_Desc& Desc = LLGL::Texture_Desc());
			static Assets::Texture& Import(const std::string& Path, const Assets::TextureUsageType& type, const LLGL::Texture_Desc& Desc = LLGL::Texture_Desc());

			static TextureImport DefaultTextureImporter;
			static MeshImport DefaultMeshImporter;

			static bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			static std::array<LLGL::Texture_Data, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const LLGL::Texture_Desc& Desc);
			
			static LLGL::Texture_Data LoadTex_stb_image(const std::string& Path, const LLGL::Texture_Desc & Desc);
			static Assets::Mesh& LoadMesh_Assimp(const std::string& Path, Assets::Material& material, const Managers::MeshLoadingDesc& desc);

		private:
			static LLGL::Texture_Data TextureCube_Load(const std::string& Path, const LLGL::Texture_Desc& Desc);
		};
	}
}