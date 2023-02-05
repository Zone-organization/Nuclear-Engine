#pragma once
#include <Assets\IAsset.h>
#include <Assets\MaterialTypes.h>
#include <Assets\Shader.h>
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
		//class Shader;

		//Defines integration between Textures & Shaders
		class NEAPI Material : public IAsset
		{
		public:
			Material();
			~Material();

			void Create(Assets::Shader* shader);

			//Note: doesnt validate the shaders!
			void BindTexSet(Graphics::ShaderPipelineVariant* pipeline ,Uint32 index);

			std::vector<MaterialTextureSet>& GetTextures();

			void SetShader(Assets::Shader* shader);
			Assets::Shader* GetShader();
			Uint32 GetShaderID();

			constexpr static auto serialize(auto& archive, auto& self)
			{
				ISLOADING(archive)
				{
					Core::UUID shaderuuid;
					auto result = archive(shaderuuid, self.mCreationShaderCommonID, self.mUsableTextures, self.mTextures);
					self.pShader = static_cast<Shader*>(Serialization::SerializationEngine::GetInstance().DeserializeUUID(AssetType::Shader, shaderuuid));
					return result;
				}
				else 
				{
					Core::UUID uuid;
					if (self.pShader != nullptr)
					{
						uuid = self.pShader->GetUUID();
					}
					return archive(uuid, self.mCreationShaderCommonID, self.mUsableTextures, self.mTextures);
				}				
			}

		private:
			Assets::Shader* pShader;
			Uint32 mCreationShaderCommonID; //Shader UUID hash
			std::vector<ShaderTextureSet> mUsableTextures;

			std::vector<MaterialTextureSet> mTextures;

			void InitializePipelineTextures();
		};
	}
}