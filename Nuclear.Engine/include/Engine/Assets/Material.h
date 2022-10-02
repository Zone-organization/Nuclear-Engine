#pragma once
#include <Engine\Assets\MaterialTypes.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>

namespace Nuclear
{
	namespace Rendering
	{
		class ShadingModel;
	}
	namespace Assets
	{

		//Describes the textures only
		class NEAPI MaterialData : public Asset<MaterialData>
		{
		public:
			std::vector<TextureSet> mTextures;

		};


		//Defines integration between Textures & Shaders
		class NEAPI Material : public Asset<Material>
		{
		public:
			Material();
			~Material();

			void Create(MaterialData* data, Rendering::ShadingModel* Pipeline);

			void BindTexSet(Uint32 index);

			Rendering::ShadingModel* GetShadingModel();
		private:

			Rendering::ShadingModel* pShaderPipeline;
			RefCntAutoPtr<IShaderResourceBinding> pShaderPipelineSRB;
			std::vector<TextureSet> mPipelineUsableTextures;
			bool mDefferedMaterial = false;
			void InitializePipelineTextures();
			MaterialData* pData;
		};
	}
}