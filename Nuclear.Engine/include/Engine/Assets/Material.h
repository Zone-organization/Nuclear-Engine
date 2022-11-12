#pragma once
#include <Engine\Assets\MaterialTypes.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>

namespace Nuclear
{
	namespace Rendering
	{
		class ShaderPipeline;
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

			void Create(MaterialData* data, Assets::Shader* shader);

			void BindTexSet(Rendering::ShaderPipeline* pipeline ,Uint32 index);

		private:
			Rendering::ShaderPipeline* pShaderPipeline;
			Uint32 mCreationShaderCommonID;
			std::vector<TextureSet> mPipelineUsableTextures;
			void InitializePipelineTextures();
			MaterialData* pData;
		};
	}
}