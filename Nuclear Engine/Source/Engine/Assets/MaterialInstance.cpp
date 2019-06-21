#include <Engine\Assets\MaterialInstance.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>
#include <Engine\Graphics\Context.h>

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
		void MaterialInstance::Create(const MICreationDesc& desc)
		{
			if (!desc.mPipeline)
			{
				Log.Error("[MaterialInstance] Creation requires a valid Pipeline object!\n");
				return;
			}
			mRenderingPipeline = desc.mPipeline;
			mRenderingPipeline->GetPipeline()->CreateShaderResourceBinding(&mSRB, true);
		}

		void MaterialInstance::Initialize(const std::vector<TextureSet>& PixelShaderTextures)
		{
			//Stage 1
			//Load Suitable & usable texture only from the main material texture set
			for (auto ShaderTexinfo : mRenderingPipeline->mPixelShaderTextureInfo)
			{
				for (auto TexSet : PixelShaderTextures)
				{
					TextureSet NewTexSet;
					for (auto TexSetTexture : TexSet)
					{
						//Found a match
						if (TexSetTexture.mTex.GetUsageType() == ShaderTexinfo.mTexture.GetUsageType())
						{
							ShaderTexture NewTex(TexSetTexture);
							TexSetTexture.mSlot = ShaderTexinfo.mSlot;
							NewTexSet.push_back(NewTex);
						}
					}
					mPShaderTextures.push_back(NewTexSet);
				}
			}

			//TODO
			//Validate the integrity of the created vector.
			//for (auto TexSet : mPShaderTextures)
			//{
			//	for (auto TexSetTexture : TexSet)
			//	{
			//		for (auto Texinfo : mRenderingPipeline->mPixelShaderTextureInfo)
			//		{
			//			if (TexSetTexture.mTexture.GetUsageType() == Texinfo.mTexture.GetUsageType())
			//			{
			//				TexSetTexture.mSlot = Texinfo.mSlot;
			//			}
			//		}
			//	}
			//}
		}

		void MaterialInstance::BindTexSet(Uint32 index)
		{
			//mPShaderTextures;
			//TODO: Check if all Slots have been occupied and then bind the free ones to fix some glitches
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