#pragma once
#include <Assets/Texture.h>
#include <Core/Path.h>
#include <Assets/Importer.h>
#include <Assets/Loader.h>

#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingModule.h>
#include <Utilities/Logger.h>

#include <Graphics\GraphicsModule.h>

#define IMPORTER_FACTORY_TYPE 1
#define LOADER_FACTORY_TYPE 2

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
		class TextureCreateTask : public Threading::MainThreadTask
		{
		public:
			//type int : 
			//1: importer
			//2: loader
			//other
			TextureCreateTask(Texture* img, TextureData* data, const Core::Path& path, const Assets::TextureDesc& desc, int factory_type)
				: pTexture(img), pImageData(data), mPath(path), mDesc(desc), mFactoryType(factory_type)
			{
			}

			bool OnRunning() override
			{
				bool result = Graphics::GraphicsModule::Get().CreateImage(pTexture, pImageData);
				
				if (mFactoryType == 1)
				{
					auto& vec = Importer::Get().GetQueuedAssets();
					for (Uint32 i = 0; i < vec.size(); i++)
					{
						if (vec.at(i)->GetUUID() == pTexture->GetUUID())
						{
							vec.erase(vec.begin() + i);
							break;
						}
					}
				}
				else if (mFactoryType == 2)
				{
					auto& vec = Loader::Get().GetQueuedAssets();
					for (Uint32 i = 0; i < vec.size(); i++)
					{
						if (vec.at(i)->GetUUID() == pTexture->GetUUID())
						{
							vec.erase(vec.begin() + i);
							break;
						}
					}
				}
				

				if (!result)
				{
					NUCLEAR_ERROR("[TextureCreateTask] Failed To Create Texture: '{0}'", mPath.GetInputPath());
					return false;
				}

				if (mFactoryType == 1)
				{
					NUCLEAR_TRACE("[Assets] Imported: {0} ", mPath.GetInputPath());
				}
				else if (mFactoryType == 2)
				{
					NUCLEAR_TRACE("[Assets] Loaded: {0} ", mPath.GetInputPath());
				}
				else
				{
					NUCLEAR_TRACE("[TextureCreateTask] Created: {0} ", mPath.GetInputPath());
				}

				return result;
			}

			void OnEnd() override
			{
				if (pImageData)
				{
					delete pImageData;
					pImageData = nullptr;
				}
				delete this;
			}

		protected:
			Assets::TextureDesc mDesc;
			Texture* pTexture;
			TextureData* pImageData;
			Core::Path mPath;
			int mFactoryType;
		};
	}
}