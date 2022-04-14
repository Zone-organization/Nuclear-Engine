#pragma once
#include <Engine/Assets/Common.h>
#include <Engine\Assets\MaterialInstance.h>
#include <unordered_map>

namespace Nuclear
{
	namespace Assets
	{
		class NEAPI Material : public Asset<Material>
		{
		public:
			Material();
			~Material();

			void CreateInstance(Rendering::ShadingModel* Pipeline);

			Assets::MaterialInstance* GetMaterialInstance(Uint32 PipelineID);

			std::vector<TextureSet> mPixelShaderTextures;

			std::string mName = std::string();
		private:
			std::unordered_map<Uint32, Assets::MaterialInstance> mMaterialInstances;
		};
	}
}