#include <Assets\Material.h>
#include <Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Graphics/GraphicsModule.h>
#include <Graphics\ShaderPipelineVariant.h>
#include <Utilities/Logger.h>
#include <Assets\Texture.h>
#include <Assets/AssetLibrary.h>

namespace Nuclear
{
	namespace Assets
	{	
		Material::Material()
			: IAsset(AssetType::Material), mCreationShaderCommonID(0), pShader(nullptr)
		{
		}
		Material::~Material()
		{
			mName = std::string();
		}

		void Material::Create(Assets::Shader* shader)
		{
			if (!shader)
			{
				NUCLEAR_ERROR("[Material] Creation requires a valid Shader Asset!");
				return;
			}
			pShader = shader;
			mCreationShaderCommonID = pShader->GetID();
			InitializePipelineTextures();
		}
		void Material::BindTexSet(Graphics::ShaderPipelineVariant* pipeline, Uint32 index)
		{
			if (!mUsableTextures.empty())
			{
				for (auto& tex : mUsableTextures.at(index))
				{
					pipeline->GetRenderingSRB()->GetVariableByIndex(Diligent::SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTex.pTexture->GetTextureView());
				}
			}

			Graphics::GraphicsModule::Get().GetContext()->CommitShaderResources(pipeline->GetRenderingSRB(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}

		std::vector<MaterialTextureSet>& Material::GetTextures()
		{
			return mTextures;
		}

		void Material::SetShader(Assets::Shader* shader)
		{
			pShader = shader;
			mCreationShaderCommonID = pShader->GetID();
		}

		Assets::Shader* Material::GetShader()
		{
			return pShader;
		}

		Uint32 Material::GetShaderID()
		{
			return mCreationShaderCommonID;
		}	

		void Material::InitializePipelineTextures()
		{
			//Stage 1
			//Load Suitable & usable texture only from the main material texture set
			for (auto& TexSet : mTextures)
			{
				ShaderTextureSet NewTexSet;
				for (auto& TexSetTexture : TexSet.mData)
				{
					for (auto& ShaderTexinfo : pShader->GetReflection().mMaterialTexturesInfo)
					{
						//Found a match
						if (TexSetTexture.mUsageType == ShaderTexinfo.mTex.mUsageType)
						{
							ShaderTexture NewTex;
							NewTex.mTex = TexSetTexture;
							NewTex.mSlot = ShaderTexinfo.mSlot;
							NewTexSet.push_back(NewTex);
						}
					}
				}
				mUsableTextures.push_back(NewTexSet);
			}

			//Stage 2 
			//Validate the integrity of the created vector, and set automatic textures if a texture isn't provided
			for (int i = 0; i < mUsableTextures.size(); i++)
			{
				//Check if a texture is missing
				if (mUsableTextures.at(i).size() != pShader->GetReflection().mMaterialTexturesInfo.size())
				{
					//Stage 2A
					//Generate a copy of the texture set that doesnt contain duplicated textures.
					auto TexSetCopy = pShader->GetReflection().mMaterialTexturesInfo;

					for (auto& ShaderTexinfo : pShader->GetReflection().mMaterialTexturesInfo)
					{
						for (int j = 0; j < mUsableTextures.at(i).size(); j++)
						{
							if (mUsableTextures.at(i).at(j).mTex.mUsageType == ShaderTexinfo.mTex.mUsageType)
							{
								for (int t = 0; t < TexSetCopy.size(); t++)
								{
									if (TexSetCopy.at(t).mTex.mUsageType == ShaderTexinfo.mTex.mUsageType)
										TexSetCopy.erase(TexSetCopy.begin() + t);
								}
							}
						}
					}

					//Stage 2B
					//Fill the orignal tex set with the missing textures from the copy.
					for (auto& MissingTex : TexSetCopy)
					{
						mUsableTextures.at(i).push_back(MissingTex);
					}
				}
			}
		}
	}
}