#include <Engine\Assets\MaterialInstance.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Assets
	{
		MaterialInstance::MaterialInstance()
			: mPShaderTextures(std::vector<MITextureSet>())
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
			//Parse loaded textures
			for (size_t i = 0; i < PixelShaderTextures.size(); i++)
			{
				for (size_t j = 0; j < PixelShaderTextures.at(i).size(); j++)
				{
					for (Graphics::ShaderTextureInfo TSinfo : mRenderingPipeline->mPixelShaderTextureInfo)
					{
						if (mPShaderTextures.at(i).at(j).mTexture.GetUsageType() == TSinfo.mTexture.GetUsageType())
						{
							mPShaderTextures.at(i).at(j).mSlot = TSinfo.mSlot;
						}
					}
				}
			}
		}

		void MaterialInstance::BindTexSet(Uint32 index)
		{
			//mPShaderTextures;
			//TODO: Check if all Slots have been occupied and then bind the free ones to fix some glitches
			if (!mPShaderTextures.empty())
			{
				for (auto tex : mPShaderTextures.at(index))
				{
					mSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTexture.mTextureView);
				}
			}

			Graphics::Context::GetContext()->CommitShaderResources(mSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
	}
}