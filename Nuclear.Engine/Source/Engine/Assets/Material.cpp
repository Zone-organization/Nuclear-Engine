#include <Engine\Assets\Material.h>
#include <Engine\Rendering\ShadingModel.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Engine\Graphics\Context.h>
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

		void Material::Create(MaterialData* data, Rendering::ShadingModel* Pipeline)
		{
			if (!Pipeline)
			{
				NUCLEAR_ERROR("[Material] Creation requires a valid Pipeline object!");
				return;
			}
			pData = data;
			pShaderPipeline = Pipeline;
			pShaderPipelineSRB = pShaderPipeline->GetActiveSRB();
			mDefferedMaterial = Pipeline->isDeffered();
			InitializePipelineTextures();
		}
		void Material::BindTexSet(Uint32 index)
		{
			if (!mPipelineUsableTextures.empty())
			{
				for (auto tex : mPipelineUsableTextures.at(index).mData)
				{
					pShaderPipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTex.GetImage()->mTextureView.RawPtr());
				}
			}

			Graphics::Context::GetContext()->CommitShaderResources(pShaderPipelineSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
		Rendering::ShadingModel* Material::GetShadingModel()
		{
			return pShaderPipeline;
		}
		void Material::InitializePipelineTextures()
		{		
			if (pData)
			{
				//Stage 1
				//Load Suitable & usable texture only from the main material texture set
				for (auto TexSet : pData->mTextures)
				{
					TextureSet NewTexSet;
					for (auto TexSetTexture : TexSet.mData)
					{
						for (auto ShaderTexinfo : pShaderPipeline->mMaterialTexturesInfo)
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
					if (mPipelineUsableTextures.at(i).mData.size() != pShaderPipeline->mMaterialTexturesInfo.size())
					{
						//Stage 2A
						//Generate a copy of the texture set that doesnt contain duplicated textures.
						auto TexSetCopy = pShaderPipeline->mMaterialTexturesInfo;

						for (auto ShaderTexinfo : pShaderPipeline->mMaterialTexturesInfo)
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
						for (auto MissingTex : TexSetCopy)
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