#include <Engine\Assets\Material.h>
#include <Engine\Rendering\ShadingModel.h>
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Assets
	{	
		Material::Material()
			: mPixelShaderTextures(std::vector<TextureSet>())
		{
		}
		Material::~Material()
		{
			mPixelShaderTextures.clear();
			mMaterialInstances.clear();
			mName = std::string();
		}
		void Material::CreateInstance(Rendering::ShadingModel* Pipeline)
		{
			MaterialInstance Instance;
			Instance.Create(Pipeline);
			Instance.Initialize(mPixelShaderTextures);
			mMaterialInstances[Pipeline->GetID()] = Instance;
		}

		Assets::MaterialInstance* Material::GetMaterialInstance(Uint32 PipelineID)
		{
			auto it = mMaterialInstances.find(PipelineID);
			if (it != mMaterialInstances.end())
			{
				return &it->second;
			}
			else
			{
				Log.Error("[Material] Instance registered to (" + std::to_string(PipelineID) + ") couldn't be found, falling back to first instance.\n");
				return &mMaterialInstances.begin()->second;
			}
		}
	}
}