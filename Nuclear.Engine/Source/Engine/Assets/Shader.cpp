#include "Engine\Assets\Shader.h"
#include <Engine/Managers/AssetManager.h>

namespace Nuclear
{
    namespace Assets
    {
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
		void ReflectData(IShaderResourceBinding* ActiveSRB, const std::string& varname, std::vector<Assets::ShaderTexture>& result, const Assets::ShaderTextureType& type)
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

		void Shader::Build(const ShaderDesc& desc)
		{
		}

		void Shader::ReflectPixelShaderData()
        {
			//pMainPipeline->
			//	
			/*	mPipeline->CreateShaderResourceBinding(&mPipelineSRB, true);

			if (mGBufferPipeline)
			{
				mGBufferPipeline->CreateShaderResourceBinding(&mGBufferSRB, true);
			}*/

			if (pMainPipeline->isDeffered())
			{
				RefCntAutoPtr<IShaderResourceBinding> GBufferSRB;
				pMainPipeline->GetGBufferPipeline()->CreateShaderResourceBinding(&GBufferSRB, true);

				ReflectData(GBufferSRB.RawPtr(), "NEMat_", mMaterialTexturesInfo, Assets::ShaderTextureType::MaterialTex);
				ReflectData(pMainPipeline->GetRenderingSRB(), "NEIBL_", mIBLTexturesInfo, Assets::ShaderTextureType::IBL_Tex);
			}
			else
			{
				ReflectData(pMainPipeline->GetRenderingSRB(), "NEMat_", mMaterialTexturesInfo, Assets::ShaderTextureType::MaterialTex);
				ReflectData(pMainPipeline->GetRenderingSRB(), "NEIBL_", mIBLTexturesInfo, Assets::ShaderTextureType::IBL_Tex);

			}

			for (Uint32 i = 0; i < pMainPipeline->GetRenderingSRB()->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = pMainPipeline->GetRenderingSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
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
							tex = &mShadowMapsInfo.mDirPos_SMInfo;
						}
						else if (VarName.find("Spot") == 0)
						{
							tex = &mShadowMapsInfo.mSpot_SMInfo;
						}
						else if (VarName.find("OmniDir") == 0)
						{
							tex = &mShadowMapsInfo.mOmniDir_SMInfo;
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
    }
}