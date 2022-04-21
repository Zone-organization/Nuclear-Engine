#include <Engine\Rendering\ShadingModel.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Engine/Managers/AssetManager.h>
namespace Nuclear
{
	namespace Rendering
	{
		void ShadingModel::Initialize(const ShadingModelInitInfo& info)
		{
			mInitInfo = info;
		}
		IPipelineState* ShadingModel::GetActivePipeline()
		{
			if (mInitInfo.mDefferedPipeline)
			{
				return GetGBufferPipeline();
			}
			return GetShadersPipeline();
		}
		IPipelineState* ShadingModel::GetShadersPipeline()
		{
			return mPipeline.RawPtr();
		}

		IShaderResourceBinding* ShadingModel::GetShadersPipelineSRB()
		{
			return mPipelineSRB.RawPtr();
		}

		IPipelineState* ShadingModel::GetGBufferPipeline()
		{
			return mGBufferPipeline.RawPtr();
		}

		Graphics::TextureUsageType ParseTexUsageFromName(std::string& name)
		{
			if (name.find("Diffuse") == 0)
				return Graphics::TextureUsageType::Diffuse;
			else if (name.find("Albedo") == 0)
				return Graphics::TextureUsageType::Albedo;
			else if (name.find("Specular") == 0)
				return Graphics::TextureUsageType::Specular;
			else if (name.find("Metallic") == 0)
				return Graphics::TextureUsageType::Metallic;
			else if (name.find("Normal") == 0)
				return Graphics::TextureUsageType::Normal;
			else if (name.find("Roughness") == 0)
				return Graphics::TextureUsageType::Roughness;
			else if (name.find("AO") == 0)
				return Graphics::TextureUsageType::AO;
			return Graphics::TextureUsageType::Unknown;
		}

		void ShadingModel::ReflectPixelShaderData()
		{
			mPipeline->CreateShaderResourceBinding(&mPipelineSRB, true);

			RefCntAutoPtr<IShaderResourceBinding> ActiveSRB;
			if (mInitInfo.mDefferedPipeline)
			{
				mGBufferPipeline->CreateShaderResourceBinding(&ActiveSRB, true);
			}
			else {
				ActiveSRB = mPipelineSRB;
			}
			for (Uint32 i = 0; i < ActiveSRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = ActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
				ShaderResourceDesc VarDesc;
				variable->GetResourceDesc(VarDesc);
				std::string VarName(VarDesc.Name);
				auto VarType = VarDesc.Type;
				if (VarType == SHADER_RESOURCE_TYPE_TEXTURE_SRV && VarName.find("NEMat_") == 0)
				{
					VarName.erase(0, 6);
					Assets::ShaderTexture ReflectedTex;
					ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
					ReflectedTex.mTex.SetName(VarName);
					ReflectedTex.mSlot = i;
					ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
					mPixelShaderTextureInfo.push_back(ReflectedTex);
				}
			}

		}
		Uint32 ShadingModel::GetID()
		{
			return mID;
		}
		Graphics::Texture ShadingModel::GetDefaultTextureFromType(Uint8 Type)
		{
			//TODO: Improve

			switch (Type)
			{
			case Graphics::TextureUsageType::Diffuse:
				Managers::AssetManager::DefaultDiffuseTex;
			case Graphics::TextureUsageType::Specular:
				Managers::AssetManager::DefaultSpecularTex;
			case Graphics::TextureUsageType::Normal:
				Managers::AssetManager::DefaultNormalTex;
			default:
				break;
			}
			return Managers::AssetManager::DefaultBlackTex;
		}
		Graphics::BakeStatus ShadingModel::GetStatus()
		{
			return mStatus;
		}
		std::unordered_map<Uint32, ShaderEffect>& ShadingModel::GetRenderingEffects()
		{
			return mRenderingEffects;
 		}
		void ShadingModel::SetEffect(const Uint32& effectId, bool value)
		{
			auto it = mRenderingEffects.find(effectId);
			if (it != mRenderingEffects.end())
			{
				it->second.SetValue(value);
			}
			else
			{
				assert(false);
			}
		}
		bool ShadingModel::isDeffered()
		{
			return mInitInfo.mDefferedPipeline;
		}
		std::string ShadingModel::GetName()
		{
			return mName;
 		}
	}
}