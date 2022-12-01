#include <Engine\Assets\Material.h>
#include <Engine\Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Graphics\ShaderPipelineVariant.h>
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Assets
	{	
		Material::Material()
		{
		}
		Material::~Material()
		{
			mName = std::string();
		}

		void Material::Create(MaterialData* data, Assets::Shader* shader)
		{
			if (!shader)
			{
				NUCLEAR_ERROR("[Material] Creation requires a valid Shader Asset!");
				return;
			}
			pData = data;
			pShader = shader;
			mCreationShaderCommonID = pShader->GetID();
			InitializePipelineTextures();

			mAlwaysRequestDefferedVariant = shader->mPipeline.GetAlwaysRequestDeffered();
		}
		void Material::BindTexSet(Graphics::ShaderPipelineVariant* pipeline, Uint32 index)
		{
			if (!mPipelineUsableTextures.empty())
			{
				for (auto& tex : mPipelineUsableTextures.at(index).mData)
				{
					pipeline->GetRenderingSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTex.GetImage()->mTextureView.RawPtr());
				}
			}

			Graphics::Context::GetInstance().GetContext()->CommitShaderResources(pipeline->GetRenderingSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}

		void Material::SetAlwaysRequestDefferedPipeline(bool val)
		{
			mAlwaysRequestDefferedVariant = val;
		}

		bool Material::GetAlwaysRequestDefferedPipeline() const
		{
			return mAlwaysRequestDefferedVariant;
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
			if (pData)
			{
				//Stage 1
				//Load Suitable & usable texture only from the main material texture set
				for (auto& TexSet : pData->mTextures)
				{
					TextureSet NewTexSet;
					for (auto& TexSetTexture : TexSet.mData)
					{
						for (auto &ShaderTexinfo : pShader->mPipeline.GetReflection().mMaterialTexturesInfo)
						{
							//Found a match
							if (TexSetTexture.mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
							{
								ShaderTexture NewTex(TexSetTexture);
								NewTex.mSlot = ShaderTexinfo.mSlot;
								NewTexSet.mData.push_back(NewTex);
							}
						}
					}
					mPipelineUsableTextures.push_back(NewTexSet);
				}

				//Stage 2 
				//Validate the integrity of the created vector, and set automatic textures if a texture isn't provided
				for (int i = 0; i < mPipelineUsableTextures.size(); i++)
				{
					//Check if a texture is missing
					if (mPipelineUsableTextures.at(i).mData.size() != pShader->mPipeline.GetReflection().mMaterialTexturesInfo.size())
					{
						//Stage 2A
						//Generate a copy of the texture set that doesnt contain duplicated textures.
						auto TexSetCopy = pShader->mPipeline.GetReflection().mMaterialTexturesInfo;

						for (auto& ShaderTexinfo : pShader->mPipeline.GetReflection().mMaterialTexturesInfo)
						{
							for (int j = 0; j < mPipelineUsableTextures.at(i).mData.size(); j++)
							{
								if (mPipelineUsableTextures.at(i).mData.at(j).mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
								{
									for (int t = 0; t < TexSetCopy.size(); t++)
									{
										if (TexSetCopy.at(t).mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
											TexSetCopy.erase(TexSetCopy.begin() + t);
									}
								}
							}
						}

						//Stage 2B
						//Fill the orignal tex set with the missing textures from the copy.
						for (auto& MissingTex : TexSetCopy)
						{
							mPipelineUsableTextures.at(i).mData.push_back(MissingTex);
						}
					}
				}

			}
			else
			{
				NUCLEAR_ERROR("[Material] InitializePipelineTextures() requires a valid MaterialData pointer!");
			}
		}
	}
}