#pragma once
#include <NE_Common.h>
#include <Assets/ImportingDescs.h>
#include <Assets/LoadingDescs.h>
#include <Assets/Importers/Model.h>

namespace Nuclear
{
	namespace Assets
	{
		namespace Importers
		{	
			class AssimpImporter;
			class AssimpManager
			{
			public:
				static AssimpManager& GetInstance();

				AssimpManager(const AssimpManager&) = delete;
				AssimpManager& operator= (const AssimpManager) = delete;

				bool Import(const std::string& importPath, const std::string& exportPath, const Model& model, const Assets::MeshImportingDesc& desc);

				bool Load(const std::string& Path, const Model& model, const Assets::MeshLoadingDesc& desc);
				bool IsExtensionSupported(const std::string& extension);

			protected:
				AssimpManager();
			};
		}
	}
}
