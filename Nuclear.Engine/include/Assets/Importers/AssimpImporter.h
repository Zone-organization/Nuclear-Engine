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
		class Model;

		namespace Importers
		{
			class NEAPI AssimpImporter
			{
			public:
				AssimpImporter();
				~AssimpImporter();

				bool isBusy();

				bool Import(const std::string& importPath, const std::string& exportPath,  Assets::Model* model, const Assets::ModelImportingDesc& desc);

				bool Load(const std::string& Path, Assets::Model* model, const Assets::ModelImportingDesc& desc);
				bool IsExtensionSupported(const std::string& extension);
				Assimp::Importer* GetImporter();
			private:
				bool busy = false;
				Assimp::Importer* pImporter;
				Assimp::Exporter* pExporter;
			};
					

		/*	class AssimpManager
			{
			public:

				std::vector<AssimpImporter> mImporters;

			};*/

			//bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::MaterialData* material, Assets::Animations* anim);
		}
	}
}
