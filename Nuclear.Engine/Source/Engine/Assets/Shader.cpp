#include "Engine\Assets\Shader.h"

namespace Nuclear
{
    namespace Assets
    {
        void Shader::ReflectPixelShaderData()
        {
			mPipeline->CreateShaderResourceBinding(&mPipelineSRB, true);

			if (mGBufferPipeline)
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
				ReflectData(mPipelineSRB.RawPtr(), "NEMat_", mMaterialTexturesInfo, Assets::ShaderTextureType::MaterialTex);
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
    }
}