#pragma once
#include <NE_Common.h>
#include <Assets/ImportingDescs.h>

struct aiMaterial;
struct aiMesh;
struct aiScene;
struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
namespace Assimp {
	class Importer;
	class Exporter;
}
namespace Nuclear
{
	namespace Assets
	{
		class Mesh;
		class Material;
		class Animations;
		namespace Importers
		{
			struct Model
			{
				std::string mName;
				Assets::Mesh* pMesh = nullptr;
				Assets::Material* pMaterial = nullptr;
				Assets::Animations* pAnimations = nullptr;
			};

			class NEAPI AssimpImporter
			{
			public:
				AssimpImporter();
				~AssimpImporter();

				bool isBusy();

				bool Import(const std::string& importPath, const std::string& exportPath,const Model& model, const Assets::MeshImportingDesc& desc);

				bool Load(const std::string& Path, const Model& model, const Assets::MeshImportingDesc& desc);
				bool IsExtensionSupported(const std::string& extension);
				Assimp::Importer* GetImporter();
			private:
				bool busy = false;
				Assimp::Importer* pImporter;
				Assimp::Exporter* pExporter;
			};
					

			class AssimpManager
			{
			public:

				std::vector<AssimpImporter> mImporters;

			};

			//bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::MaterialCreationInfo* material, Assets::Animations* anim);
		}
	}
}
