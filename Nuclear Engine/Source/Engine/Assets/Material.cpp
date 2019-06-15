#include <Engine\Assets\Material.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Assets
	{	
		Material::Material()
			: mPixelShaderTextures(std::vector<TextureSet>()), mPixelShaderTS(std::vector<ShaderTexture>())
		{
		}
		Material::~Material()
		{
		}
		void Material::Create(const MaterialCreationDesc& desc)
		{
			if (!desc.mPipeline)
			{
				Log.Error("[Material] Creation requires a valid Pipeline object!\n");
				return;
			}
			mPipeline = desc.mPipeline;
			mPipeline->CreateShaderResourceBinding(&mSRB, true);

			ParseShader(SHADER_TYPE_PIXEL);
		}


		void Material::BindTexSet(Uint32 index)
		{
			//mPixelShaderTextures;
			//TODO: Check if all Slots have been occupied and then bind the free ones to fix some glitches
			if (!mPixelShaderTextures.empty())
			{
				for (auto tex : mPixelShaderTextures.at(index))
				{	
					mSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, tex.mSlot)->Set(tex.mTex.mTextureView);
				}
			}

			Graphics::Context::GetContext()->CommitShaderResources(mSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}

		void Material::Bind()
		{	
			//Graphics::Context::GetContext()->SetPipelineState(mPipeline);
		}

		IPipelineState * Material::GetPipeline()
		{
			return mPipeline;
		}

		TextureUsageType ParseTexUsageFromName(std::string& name)
		{
			if (name.find("Diffuse") == 0)
				return TextureUsageType::Diffuse;
			else if (name.find("Specular") == 0)
				return TextureUsageType::Specular;
			else if (name.find("Normal") == 0)
				return TextureUsageType::Normal;

			return TextureUsageType::Unknown;
		}

		void Material::ParseShader(SHADER_TYPE shadertype)
		{
			for (Uint32 i = 0; i < mSRB->GetVariableCount(shadertype); i++)
			{
				auto variable = mSRB->GetVariableByIndex(shadertype, i);
				std::string VariableName(variable->GetResourceDesc().Name);

				if (variable->GetResourceDesc().Type == SHADER_RESOURCE_TYPE_TEXTURE_SRV && VariableName.find("NEMat_") == 0)
				{
					VariableName.erase(0, 6);
					ShaderTexture mTex;
					mTex.mTex = DefaultTextures::DefaultBlackTex;
					mTex.mTex.SetName(VariableName);
					mTex.mSlot = i;
					mTex.mTex.SetUsageType(ParseTexUsageFromName(VariableName));
					mPixelShaderTS.push_back(mTex);
				}			
			}

			//Parse loaded textures
			for (size_t i = 0; i < mPixelShaderTextures.size(); i++)
			{
				for (size_t j = 0; j < mPixelShaderTextures.at(i).size(); j++)
				{
					for (ShaderTexture TSinfo : mPixelShaderTS)
					{
						if (mPixelShaderTextures.at(i).at(j).mTex.GetUsageType() == TSinfo.mTex.GetUsageType())
						{
							mPixelShaderTextures.at(i).at(j).mSlot = TSinfo.mSlot;
						}
					}
				}
			}
		}
	}
}