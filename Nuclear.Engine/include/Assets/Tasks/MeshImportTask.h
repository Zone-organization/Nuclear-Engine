#pragma once
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Assets/ImportingDescs.h>
#include <filesystem>
#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>
#include <Assets\Importers\AssimpManager.h>

#include <Assets/AssetManager.h>

#include <Platform/FileSystem.h>
#include <Utilities/Logger.h>

#include <Serialization/SerializationEngine.h>

#include <Assets/Tasks/MeshCreateTask.h>

namespace Nuclear
{
	namespace Assets
	{
		class MeshImportTask : public Threading::Task
		{
		public:
			MeshImportTask(const Importers::Model& model, const Core::Path& path, const Assets::MeshImportingDesc& desc)
				: mResult(model), mPath(path), mImportingDesc(desc)
			{
			}
			~MeshImportTask()
			{

			}
			bool OnRunning() override
			{
				std::string exportpath;

				if (mImportingDesc.mCommonOptions.mExportPath.isValid())
				{
					exportpath = mImportingDesc.mCommonOptions.mExportPath.GetRealPath();
				}
				else
				{
					exportpath = AssetLibrary::GetInstance().GetPath() + "Meshes/" + mResult.mName + '/';
				}

				Platform::FileSystem::GetInstance().CreateFolders(exportpath + "Textures/");

				if (!Importers::AssimpManager::GetInstance().Import(mPath.GetRealPath(), exportpath, mResult, mImportingDesc))
				{
					NUCLEAR_ERROR("[Importer] Failed to import Model : '{0}'", mPath.GetInputPath());

					return false;
				}

				if (mResult.pMaterial && mImportingDesc.pMaterialShader)
				{
					mResult.pMaterial->Create(mImportingDesc.pMaterialShader);
				}

				if (!mImportingDesc.mCommonOptions.mLoadOnly)
				{
					if (mImportingDesc.mExportMaterial && mResult.pMaterial)
					{
						AssetManager::GetInstance().Export(mResult.pMaterial, true, exportpath);
					}
				}

				//TODO: Export Animations?
				//if (mImportingDesc.mImportAnimations && mResult.pAnimations)
				//{
				//	AssetManager::GetInstance().Export(mResult.pAnimations, true, exportpath);
				//}


				//should do for meshes and materials?
				if (mImportingDesc.mImportAnimations)
				{
					if (mResult.pAnimations)
					{
						if (mResult.pAnimations->GetState() != IAsset::State::Loaded)
						{
							AssetLibrary::GetInstance().mImportedAnimations.mData.erase(mResult.pAnimations->GetUUID());
							mResult.pAnimations = nullptr;
						}
						else
						{
							mResult.pAnimations->SetState(IAsset::State::Loaded);
						}
					}
				}

				if (mResult.pMesh)
				{
					//create task
					Threading::ThreadingEngine::GetInstance().AddMainThreadTask(new MeshCreateTask(mResult.pMesh, mPath, IMPORTER_FACTORY_TYPE));

					if (!mImportingDesc.mCommonOptions.mLoadOnly)
					{
						AssetMetadata assetmetadata = Assets::AssetManager::GetInstance().CreateMetadata(mResult.pMesh);

						auto meshloadingdesc = static_cast<Assets::MeshLoadingDesc*>(assetmetadata.pLoadingDesc = new Assets::MeshLoadingDesc);
						meshloadingdesc->mSaveMaterialNames = mImportingDesc.mSaveMaterialNames;
						meshloadingdesc->mExternalMaterial = mImportingDesc.mExportMaterial;

						if (mResult.pMaterial)
							meshloadingdesc->mMaterialUUID = mResult.pMaterial->GetUUID();

						if (mResult.pAnimations)
							meshloadingdesc->mAnimationsUUID = mResult.pAnimations->GetUUID();

						//Export Meta
						Serialization::SerializationEngine::GetInstance().Serialize(assetmetadata, exportpath + mResult.mName + ".glb" + ".NEMeta");
						delete assetmetadata.pLoadingDesc;
					}
				}						

				return true;
			}

			void OnEnd() override
			{
				delete this;
			}
		protected:
			Importers::Model mResult;
			Assets::MeshImportingDesc mImportingDesc;
			Core::Path mPath;
		};

	}
}