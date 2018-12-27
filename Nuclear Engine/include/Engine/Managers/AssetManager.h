#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Assets/Texture.h>
#include <Engine/Assets/Image.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
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
		struct TextureLoadingDesc
		{
			std::string mName;

			Diligent::USAGE mUsage;

			Diligent::BIND_FLAGS mBindFlags;

			Uint32 mMipLevels;

			Diligent::CPU_ACCESS_FLAGS mCPUAccessFlags;

			bool mIsSRGB;

			bool mGenerateMips;

			bool mFlipY_Axis;

			Diligent::TEXTURE_FORMAT mFormat;

			TextureLoadingDesc() :
				mName(""),
				mUsage(Diligent::USAGE_STATIC),
				mBindFlags(Diligent::BIND_SHADER_RESOURCE),
				mMipLevels(0),
				mCPUAccessFlags(Diligent::CPU_ACCESS_NONE),
				mIsSRGB(false),
				mGenerateMips(true),
				mFlipY_Axis(true),
				mFormat(Diligent::TEX_FORMAT_UNKNOWN)
			{}
		};

		typedef Utilities::Delegate<Assets::Mesh&(const std::string& Path, const Managers::MeshLoadingDesc& desc)> MeshImport;
		typedef Utilities::Delegate<Assets::Image&(const std::string& Path, const TextureLoadingDesc & Desc)> TextureImport;

		class NEAPI AssetManager {
		public:
			AssetManager();
			~AssetManager();

			//All loaded textures with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Assets::Texture> mImportedTextures;
			//Real pre-hashed texture names with paths (conditionally saved see SaveTextureNames)
			std::unordered_map<Uint32, std::string> mHashedTexturesNames;
			//tells the asset manager whether to store the real texture name or not
			bool mSaveTextureNames = DEBUG_TRUE_BOOL;

			//All imported meshes with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Assets::Mesh> mImportedMeshes;
			//Real pre-hashed meshes names with paths (conditionally saved see SaveTextureNames)
			std::unordered_map<Uint32, std::string> mHashedMeshesNames;
			//tells the asset manager whether to store the real mesh name or not
			bool mSaveMeshNames = DEBUG_TRUE_BOOL;

			//All imported materials with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Assets::Material> mImportedMaterials;
			//Real pre-hashed meshes names with paths (conditionally saved see SaveTextureNames)
			std::unordered_map<Uint32, std::string> mHashedMaterialsNames;
			//tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsNames = DEBUG_TRUE_BOOL;

			//Methods
			void Initialize(bool SaveTextureNames = false);
			void ShutDown();

			Assets::Mesh& Import(const std::string& Path, const MeshLoadingDesc& desc, Assets::Material& material);
			Assets::Texture& Import(const std::string& Path, const TextureLoadingDesc& Desc = TextureLoadingDesc());
			Assets::Texture& Import(const std::string& Path, const Assets::TextureUsageType& type, const TextureLoadingDesc& Desc = TextureLoadingDesc());

			TextureImport DefaultTextureImporter;
			MeshImport DefaultMeshImporter;

			bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<Diligent::ITexture*, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const TextureLoadingDesc& Desc);

			static Assets::Image& LoadTex_STB(const std::string& Path, const TextureLoadingDesc & Desc);
			static Assets::Mesh& LoadMesh_Assimp(const std::string& Path, Assets::Material& material, const Managers::MeshLoadingDesc& desc);

		private:
			Diligent::ITexture* TextureCube_Load(const std::string& Path, const TextureLoadingDesc& Desc);
		};
	}
}