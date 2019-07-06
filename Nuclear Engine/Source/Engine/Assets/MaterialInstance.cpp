#include <Engine\Assets\MaterialInstance.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>

namespace NuclearEngine
{
	namespace Assets
	{
		MaterialInstance::MaterialInstance()
			: mPShaderTextures(std::vector<TextureSet>())
		{
		}
		MaterialInstance::~MaterialInstance()
		{
		}
		void MaterialInstance::Create(Graphics::RenderingPipeline* Pipeline)
		{
			if (!Pipeline)
			{
				Log.Error("[MaterialInstance] Creation requires a valid Pipeline object!\n");
				return;
			}
			mRenderingPipeline =Pipeline;
			mRenderingPipeline->GetPipeline()->CreateShaderResourceBinding(&mSRB, true);
		}

		void MaterialInstance::Initialize(const std::vector<TextureSet>& PixelShaderTextures)
		{
			//Stage 1
			//Load Suitable & usable texture only from the main material texture set
			for (auto TexSet : PixelShaderTextures)
			{
				TextureSet NewTexSet;
				for (auto TexSetTexture : TexSet.mData)
				{
					for (auto ShaderTexinfo : mRenderingPipeline->mPixelShaderTextureInfo)
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
				mPShaderTextures.push_back(NewTexSet);
			}
			
			//Stage 2 
			//Validate the integrity of the created vector, and set automatic textures if a texture isn't provided
			for (int i = 0; i < mPShaderTextures.size(); i++)
			{
				//Check if a texture is missing
				if (mPShaderTextures.at(i).mData.size() != mRenderingPipeline->mPixelShaderTextureInfo.size())
				{				
					//Stage 2A
					//Generate a copy of the texture set that doesnt contain duplicated textures.
					auto TexSetCopy = mRenderingPipeline->mPixelShaderTextureInfo;

					for (auto ShaderTexinfo : mRenderingPipeline->mPixelShaderTextureInfo)
					{
						for (int j = 0; j < mPShaderTextures.at(i).mData.size(); j++)
						{
							if (mPShaderTextures.at(i).mData.at(j).mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
							{
								for (int t = 0; t < TexSetCopy.size(); t++)
								{
									if (TexSetCopy.at(t).mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
										TexSetCopy.erase(TexSetCopy.begin()  + t);
								}
							}
						}
					}

					//Stage 2B
					//Fill the orignal tex set with the missing textures from the copy.
					for (auto MissingTex : TexSetCopy)
					{
						mPShaderTextures.at(i).mData.push_back(MissingTex);
					}
				}
			}
			
		}
		void MaterialInstance::BindTexSet(Uint32 index)
		{
			if (!mPShaderTextures.empty())
			{
				for (auto tex : mPShaderTextures.at(index).mData)
				{
					mSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTex.mTextureView);
				}
			}

			Graphics::Context::GetContext()->CommitShaderResources(mSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
	
	}
}