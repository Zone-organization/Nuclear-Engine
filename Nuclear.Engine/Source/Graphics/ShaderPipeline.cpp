#include <Graphics/ShaderPipeline.h>
#include <Graphics/ShaderPipelineVariantFactory.h>
#include <Utilities/Hash.h>
#include <Utilities/Logger.h>
#include <Assets/Shader.h>

namespace Nuclear
{
	namespace Graphics
	{
		using namespace Diligent;
		
		ShaderPipeline::ShaderPipeline(Assets::Shader* parent)
			: pParentShader(parent), mDesc(pParentShader->mBuildDesc.mPipelineDesc)
		{
		}

		ShaderPipeline::ShaderPipeline(ShaderPipelineDesc& desc)
			: pParentShader(nullptr), mDesc(desc)
		{
		}

		ShaderPipeline::~ShaderPipeline()
		{
			mVariants.clear();
		}

		void ShaderPipeline::BuildVariants()
		{
			ShaderPipelineVariantDesc ZeroInstance;
			ZeroInstance.mHashKey = 0;
			if (pParentShader && pParentShader->mBuildDesc.mSupportedTechniques == Graphics::SupportedRenderingTechnique::DefferedOnly)
			{
				ZeroInstance.isDeffered = true;
			}
			mVariantsInfo.push_back(ZeroInstance);

			//Phase 1: Process Switches
			for (Uint32 ISwitch = 0; ISwitch < mDesc.Switches.size(); ISwitch++)
			{
				for (Uint32 NextSwitch = 0; NextSwitch < mDesc.Switches.size(); NextSwitch++)
				{
					for (Uint32 NextSwitch2 = 0; NextSwitch2 < mDesc.Switches.size(); NextSwitch2++)
					{
						ShaderPipelineVariantDesc Info_;
						Info_.mDefines.insert(mDesc.Switches.at(ISwitch).GetName());
						Info_.mDefines.insert(mDesc.Switches.at(NextSwitch).GetName());
						Info_.mDefines.insert(mDesc.Switches.at(NextSwitch2).GetName());

						for (auto& i : Info_.mDefines)
						{
							auto iHash = Utilities::Hash(i);
							Info_.mHashKey = Info_.mHashKey + iHash;
							if (i == "NE_DEFFERED")
							{
								//mReflection.mHasDefferedPipelines = true;
								Info_.isDeffered = true;
							}
							else if (i == "NE_ANIMATION")
							{
								Info_.isSkinned = true;
							}
							else if (i == "NE_SHADOWS")
							{
								Info_.isShadowed = true;
							}
						}

						int Found = 0;

						for (auto& i : mVariantsInfo)
						{
							if (i.mHashKey == Info_.mHashKey)
							{
								Found = Found + 1;
							}
						}

						if (Found == 0)
						{
							mVariantsInfo.push_back(Info_);
						}
					}
				}
			}
		}

		bool ShaderPipeline::Bake(ShaderPipelineBakingDesc& bakingdesc)
		{
			if (pParentShader->mBuildDesc.mType == ShaderType::Rendering3D)
			{
				bool hasdeffered = false;
				for (auto& Info : mVariantsInfo)
				{
					if (Info.isDeffered)
					{
						auto& variant = mVariants[Info.mHashKey];
						variant.mDesc = Info;
						bakingdesc.pVariantsFactory->CreateDefferedVariant(variant, *this, bakingdesc);
						hasdeffered = true;
					}
					else
					{
						auto& variant = mVariants[Info.mHashKey];
						variant.mDesc = Info;
						bakingdesc.pVariantsFactory->CreateForwardVariant(variant, *this, bakingdesc);
					}
				}

				if (hasdeffered)
				{
					mGBuffer.Bake(Math::Vector2ui(bakingdesc.mRTWidth, bakingdesc.mRTHeight));
				}
				return true;
			}
			else if (pParentShader->mBuildDesc.mType == ShaderType::RenderingEffect)
			{
				for (auto& Info : mVariantsInfo)
				{
					auto& variant = mVariants[Info.mHashKey];
					variant.mDesc = Info;
					bakingdesc.pVariantsFactory->CreateForwardVariant(variant,*this, bakingdesc);
				}
			}

			return false;
		}

		Uint32 ShaderPipeline::GetHashedKey(const std::string& Key)
		{
			auto KeyHash = Utilities::Hash(Key);
			for (auto& i : mVariants)
			{
				if (i.first == KeyHash)
				{
					return KeyHash;
				}
			}

			//Error
			return 0;
		}
		ShaderPipelineVariant* ShaderPipeline::GetVariant(Uint32 Key)
		{
			for (auto& i : mVariants)
			{
				if (i.first == Key)
				{
					return &i.second;
				}
			}

			NUCLEAR_ERROR("[ShaderPipeline] Variant: '{0}' Not Found!", Key);

			if (mVariants.size() > 0)
				return &mVariants.begin()->second;

			return nullptr;
		}

		const std::vector<ShaderPipelineSwitch>& ShaderPipeline::GetSwitches() const
		{
			return mDesc.Switches;
		}

		Assets::Shader* ShaderPipeline::GetShaderAsset()
		{
			return pParentShader;
		}

		Rendering::GBuffer* ShaderPipeline::GetGBuffer()
		{
			return &mGBuffer;
		}
	}
}