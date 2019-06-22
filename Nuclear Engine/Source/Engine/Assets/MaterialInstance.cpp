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
				for (auto TexSetTexture : TexSet)
				{
					for (auto ShaderTexinfo : mRenderingPipeline->mPixelShaderTextureInfo)
					{
						//Found a match
						if (TexSetTexture.mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
						{
							ShaderTexture NewTex(TexSetTexture);
							NewTex.mSlot = ShaderTexinfo.mSlot;
							NewTexSet.push_back(NewTex);
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
				if (mPShaderTextures.at(i).size() != mRenderingPipeline->mPixelShaderTextureInfo.size())
				{				
					//Stage 2A
					//Generate a copy of the texture set that doesnt contain duplicated textures.
					auto TexSetCopy = mRenderingPipeline->mPixelShaderTextureInfo;

					for (auto ShaderTexinfo : mRenderingPipeline->mPixelShaderTextureInfo)
					{
						for (int j = 0; j < mPShaderTextures.at(i).size(); j++)
						{
							if (mPShaderTextures.at(i).at(j).mTex.GetUsageType() == ShaderTexinfo.mTex.GetUsageType())
							{
								TexSetCopy.erase(TexSetCopy.begin() + j);
							}
						}
					}

					//Stage 2B
					//Fill the orignal tex set with the missing textures from the copy.
					for (auto MissingTex : TexSetCopy)
					{
						mPShaderTextures.at(i).push_back(MissingTex);
					}
				}
			}
			
		}
		void MaterialInstance::ExtractTextureSetInfo(const std::vector<TextureSet>& data)
		{
			Log.Info("====   TexSet Info Begin...   ====\n");
			Log.Info("Tex Set Size: " + std::to_string(data.size()) + " \n");

			for (int i = 0; i < data.size(); i++)
			{
				Log.Info("-TexSet Index [" + std::to_string(i) + "] Size: " + std::to_string(data[i].size()) + " Begin: \n");

				for (int j = 0; j < data[i].size(); j++)
				{
					Log.Info("---Texture Index [" + std::to_string(j) + " Info:\n");
					Log.Info("-----Texture Slot [" + std::to_string(data[i][j].mSlot) + "] \n");
					Log.Info("-----Texture Type [" + std::to_string(data[i][j].mTex.GetUsageType()) + "] \n");

				}

				Log.Info("-TexSet Index [" + std::to_string(i) + "] END... \n");

			}

			Log.Info("====   TexSet Info End...   ====\n");

		}
		void MaterialInstance::BindTexSet(Uint32 index)
		{
			if (!mPShaderTextures.empty())
			{
				for (auto tex : mPShaderTextures.at(index))
				{
					mSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTex.mTextureView);
				}
			}

			Graphics::Context::GetContext()->CommitShaderResources(mSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
	
	}
}