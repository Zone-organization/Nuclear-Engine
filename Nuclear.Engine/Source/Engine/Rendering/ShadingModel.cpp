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
		IShaderResourceBinding* ShadingModel::GetActiveSRB()
		{
			if (mInitInfo.mDefferedPipeline)
			{
				return GetGBufferPipelineSRB();
			}
			return GetShadersPipelineSRB();
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

		IShaderResourceBinding* ShadingModel::GetGBufferPipelineSRB()
		{
			return mGBufferSRB.RawPtr();
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

			//IBL
			else if (name.find("IrradianceMap") == 0)
				return Graphics::TextureUsageType::IrradianceMap;
			else if (name.find("PrefilterMap") == 0)
				return Graphics::TextureUsageType::PreFilterMap;
			else if (name.find("BRDF_LUT") == 0)
				return Graphics::TextureUsageType::BRDF_LUT;

			//Shadows
			else if (name.find("ShadowMap") == 0)
				return Graphics::TextureUsageType::ShadowTex;

			return Graphics::TextureUsageType::Unknown;
		}


		void ReflectData(IShaderResourceBinding* ActiveSRB,const std::string& varname, std::vector<Assets::ShaderTexture>& result,const Assets::ShaderTextureType& type)
		{
			for (Uint32 i = 0; i < ActiveSRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = ActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
				ShaderResourceDesc VarDesc;
				variable->GetResourceDesc(VarDesc);
				std::string VarName(VarDesc.Name);
				auto VarType = VarDesc.Type;
				if (VarType == SHADER_RESOURCE_TYPE_TEXTURE_SRV && VarName.find(varname) == 0)
				{
					VarName.erase(0, varname.size());

					Assets::ShaderTexture ReflectedTex;
					ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
					ReflectedTex.mTex.SetName(VarName);
					ReflectedTex.mSlot = i;
					ReflectedTex.mType = type;
					ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
					result.push_back(ReflectedTex);
				}
			}
		}
		void ShadingModel::ReflectPixelShaderData()
		{
			mPipeline->CreateShaderResourceBinding(&mPipelineSRB, true);

			if(mGBufferPipeline)
			{
				mGBufferPipeline->CreateShaderResourceBinding(&mGBufferSRB, true);
			}
			if (mInitInfo.mDefferedPipeline)
			{
				RefCntAutoPtr<IShaderResourceBinding> GBufferSRB;
				mGBufferPipeline->CreateShaderResourceBinding(&GBufferSRB, true);

				ReflectData(GBufferSRB.RawPtr(), "NEMat_", mMaterialTexturesInfo, Assets::ShaderTextureType::MaterialTex);
				ReflectData(mPipelineSRB.RawPtr(), "NEIBL_", mIBLTexturesInfo, Assets::ShaderTextureType::IBL_Tex);
			}
			else
			{
				ReflectData(mPipelineSRB.RawPtr(),"NEMat_", mMaterialTexturesInfo, Assets::ShaderTextureType::MaterialTex);
				ReflectData(mPipelineSRB.RawPtr(), "NEIBL_", mIBLTexturesInfo, Assets::ShaderTextureType::IBL_Tex);

			}

			for (Uint32 i = 0; i < mPipelineSRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = mPipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
				ShaderResourceDesc VarDesc;
				variable->GetResourceDesc(VarDesc);
				std::string VarName(VarDesc.Name);
				auto VarType = VarDesc.Type;
				if (VarType == SHADER_RESOURCE_TYPE_TEXTURE_SRV)
				{
					//Shadow Maps
					if (VarName.find("NE_ShadowMap") == 0)
					{
						VarName.erase(0, 13);

						Assets::ShaderTexture* tex;
						if (VarName.find("DirPos") == 0)
						{
							tex = &mDirPos_ShadowmapInfo;
						}
						else if (VarName.find("Spot") == 0)
						{
							tex = &mSpot_ShadowmapInfo;
						}
						else if (VarName.find("OmniDir") == 0)
						{
							tex = &mOmniDir_ShadowmapInfo;
						}
						else {
							assert(false);
						}
						tex->mTex = Managers::AssetManager::DefaultWhiteTex;
						tex->mTex.SetName(VarName);
						tex->mSlot = i;
						tex->mType = Assets::ShaderTextureType::ShadowTex;
						tex->mTex.SetUsageType(ParseTexUsageFromName(VarName));
					}
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
		std::vector<Graphics::RenderTargetDesc> ShadingModel::GetGBufferDesc()
		{
			return std::vector<Graphics::RenderTargetDesc>();
		}
		std::string ShadingModel::GetName()
		{
			return mName;
 		}


		void ShadingModel::AddToDefinesIfNotZero(std::vector<std::string>& defines, const std::string& name, Uint32 value)
		{
			if (value > 0) { defines.push_back(name + std::to_string(value)); }
		}

	}
}