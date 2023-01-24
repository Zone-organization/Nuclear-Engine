#pragma once
#include <Assets/Texture.h>
#include <Core/Path.h>

#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>
#include <Utilities/Logger.h>

#include <Graphics\GraphicsEngine.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;

		class TextureCreateTask : public Threading::MainThreadTask
		{
		public:
			TextureCreateTask(Texture* img, TextureData* data, const Core::Path& path, const Assets::TextureDesc& desc, std::vector<IAsset*>* queued_assets_vec)
				: pTexture(img), pImageData(data), mPath(path), mDesc(desc), pQueuedAssets(queued_assets_vec)
			{
			}

			bool OnRunning() override
			{
				bool result = Graphics::GraphicsEngine::GetInstance().CreateImage(pTexture, pImageData);
				
				if (pQueuedAssets)
				{
					for (Uint32 i = 0; i < pQueuedAssets->size(); i++)
					{
						if (pQueuedAssets->at(i)->GetUUID() == pTexture->GetUUID())
						{
							pQueuedAssets->erase(pQueuedAssets->begin() + i);
							break;
						}
					}
				}

				if (!result)
				{
					NUCLEAR_ERROR("[Importer] Failed To Create Texture: '{0}'", mPath.GetInputPath());
					return false;
				}

				NUCLEAR_INFO("[Assets] Imported: {0} ", mPath.GetInputPath());
				return result;
			}

			void OnEnd() override
			{
				delete pImageData;
				delete this;
			}

		protected:
			Assets::TextureDesc mDesc;
			Texture* pTexture;
			TextureData* pImageData;
			Core::Path mPath;
			std::vector<IAsset*>* pQueuedAssets;
		};
	}
}