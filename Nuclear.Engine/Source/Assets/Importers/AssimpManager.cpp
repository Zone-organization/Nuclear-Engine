#include <Assets\Importers\AssimpManager.h>
#include "AssimpImporter.h"

namespace Nuclear
{
	namespace Assets
	{
		namespace Importers
		{
			AssimpManager::AssimpManager()
			{
			}

			AssimpManager& Importers::AssimpManager::Get()
			{
				static AssimpManager instance;
				return instance;
			}

			bool AssimpManager::Import(const std::string& importPath, const std::string& exportPath, const Model& model, const Assets::MeshImportingDesc& desc)
			{
				AssimpImporter instance;
				return instance.Import(importPath, exportPath, model, desc);
			}

			bool AssimpManager::Load(const std::string& Path, const Model& model, const Assets::MeshLoadingDesc& desc)
			{
				AssimpImporter instance;
				return instance.Load(Path, model, desc);
			}

			bool AssimpManager::IsExtensionSupported(const std::string& extension)
			{
				return AssimpImporter::IsExtensionSupported(extension);
			}
		}
	}
}