#pragma once
#include <Assets\MaterialTypes.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>

namespace Nuclear
{
	namespace Graphics
	{
		class ShaderPipelineVariant;
	}
	namespace Assets
	{
		class Shader;

		//Describes the textures only
		class NEAPI MaterialData : public IAsset
		{
		public:
			MaterialData()
				: IAsset(AssetType::MaterialData)
			{

			}

			std::vector<TextureSet> mTextures;

		};

		//Defines integration between Textures & Shaders
		class NEAPI Material : public IAsset
		{
		public:
			Material();
			~Material();

			void Create(MaterialData* data, Assets::Shader* shader);

			//Note: doesnt validate the shaders!
			void BindTexSet(Graphics::ShaderPipelineVariant* pipeline ,Uint32 index);

			Assets::Shader* GetShader();
			Uint32 GetShaderID();

		private:
			Assets::Shader* pShader;
			Uint32 mCreationShaderCommonID;
			std::vector<TextureSet> mPipelineUsableTextures;
			void InitializePipelineTextures();
			MaterialData* pData;
		};
	}
}