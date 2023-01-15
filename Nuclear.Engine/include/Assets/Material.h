#pragma once
#include <Assets\IAsset.h>
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

		//Defines integration between Textures & Shaders
		class NEAPI Material : public IAsset
		{
		public:
			Material();
			~Material();

			void Create(const MaterialCreationInfo& desc, Assets::Shader* shader);

			//Note: doesnt validate the shaders!
			void BindTexSet(Graphics::ShaderPipelineVariant* pipeline ,Uint32 index);

			Assets::Shader* GetShader();
			Uint32 GetShaderID();

		private:
			Assets::Shader* pShader;		//Asset UUID
			Uint32 mCreationShaderCommonID; //Shader UUID hash
			std::vector<ShaderTextureSet> mMaterialShaderTextures;

			void InitializePipelineTextures(const MaterialCreationInfo& desc);
		};
	}
}