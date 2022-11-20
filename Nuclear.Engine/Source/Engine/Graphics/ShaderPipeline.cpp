#include <Engine\Graphics\ShaderPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Core/Utilities/Hash.h>
#include <Core\Logger.h>
#include <Engine/Managers/AssetManager.h>

namespace Nuclear
{
	namespace Graphics
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

		std::string MergeCode(const std::string& shadersource, const std::set<std::string>& defines)
		{
			std::vector<std::string> MergedCode;

			for (auto i : defines)
			{
				MergedCode.push_back("#define " + i + "\n");
			}
			std::string str;
			MergedCode.push_back(shadersource);
			for (auto i : MergedCode)
			{
				str = str + i;
			}
			return str;
		}

		ShaderPipeline::ShaderPipeline(Assets::Shader* parent)
			: mParentAsset(parent)
		{
		}

		ShaderPipeline::~ShaderPipeline()
		{
			mSwitches.clear();
			mVariants.clear();
		}

		void ShaderPipeline::Create(const ShaderPipelineDesc& Desc)
		{
			mDesc = Desc;
			ShaderPipelineVariantDesc ZeroInstance;
			ZeroInstance.mHashKey = 0;

			mVariantsInfo.push_back(ZeroInstance);

			//Phase 1: Process Switches
			for (Uint32 ISwitch = 0; ISwitch < Desc.Switches.size(); ISwitch++)
			{
				for (Uint32 NextSwitch = 0; NextSwitch < Desc.Switches.size(); NextSwitch++)
				{
					for (Uint32 NextSwitch2 = 0; NextSwitch2 < Desc.Switches.size(); NextSwitch2++)
					{
						ShaderPipelineVariantDesc Info_;
						Info_.mDefines.insert(Desc.Switches.at(ISwitch).GetName());
						Info_.mDefines.insert(Desc.Switches.at(NextSwitch).GetName());
						Info_.mDefines.insert(Desc.Switches.at(NextSwitch2).GetName());

						for (auto& i : Info_.mDefines)
						{
							auto iHash = Utilities::Hash(i);
							Info_.mHashKey = Info_.mHashKey + iHash;

							//TODO: Is Correct????
							if (i == "DEFFERED")
							{
								Info_._isDeffered = true;
							}
							else if (i == "ANIMATION")
							{
								Info_._isSkinned = true;
							}
							else if (i == "SHADOWS")
							{
								Info_._isShadowed = true;
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

		bool ShaderPipeline::Bake(ShaderRenderingBakingDesc* bakingdesc)
		{
			//Phase 2: Create Pipelines
			for (auto& Info : mVariantsInfo)
			{
				if (Info._isDeffered)
				{
					mVariants[Info.mHashKey] = CreateDefferedVariant(Info, mDesc);
				}
				else {
					mVariants[Info.mHashKey] = CreateForwardVariant(Info, mDesc);
				}
			}

			//Phase 3: Reflection
			if (mParentAsset)
			{
				auto& firstinstance = mVariants.at(0);


			}
			return false;
		}

		ShaderPipelineVariant ShaderPipeline::CreateForwardVariant(const ShaderPipelineVariantDesc& Info, const ShaderPipelineDesc& Desc)
		{
			ShaderPipelineVariant result;

			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;
			GraphicsEngine::GetShaderManager()->CreateShader(VShader.RawDblPtr(), Desc.mForwardPSOCreateInfo.mVertexShader);
			GraphicsEngine::GetShaderManager()->CreateShader(PShader.RawDblPtr(), Desc.mForwardPSOCreateInfo.mPixelShader);
		//	GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(Desc.mForwardPSOCreateInfo.mVertexShader.mSource, Info.mDefines), , SHADER_TYPE_PIXEL);

			GraphicsPipelineStateCreateInfo PSOCreateInfo;
			std::string psoname(Desc.mName + "_ID_" + std::to_string(Info.mHashKey));
			PSOCreateInfo.PSODesc.Name = psoname.c_str();
			PSOCreateInfo.GraphicsPipeline = Desc.mForwardPSOCreateInfo.GraphicsPipeline;
			PSOCreateInfo.pVS = VShader;
			PSOCreateInfo.pPS = PShader;

			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&result.mPipeline, PSOCreateInfo, Vars, true);

			result.mPipeline->CreateShaderResourceBinding(&result.mPipelineSRB, true);

			return result;
		}

		ShaderPipelineVariant ShaderPipeline::CreateDefferedVariant(const ShaderPipelineVariantDesc& variantdesc, const ShaderPipelineDesc& pipelinedesc)
		{
			//TODO
			return ShaderPipelineVariant();
		}

		Uint32 ShaderPipeline::GetHashedKey(const std::string& Key)
		{
			auto KeyHash = Utilities::Hash(Key);
			for (auto i : mVariants)
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

			if(mVariants.size() > 0)
				return &mVariants.begin()->second;

			return nullptr;
		}

		const std::vector<ShaderPipelineSwitch>& ShaderPipeline::GetSwitches() const
		{
			return mSwitches;
		}

		void ShaderPipeline::ReflectPixelShaderData(ShaderPipelineVariant& pipeline)
		{
			if (pipeline.isDeffered())
			{
				ReflectData(pipeline.GetGBufferPipelineSRB(), "NEMat_", mParentAsset->mReflection.mMaterialTexturesInfo, Assets::ShaderTextureType::MaterialTex);
			}

			for (Uint32 i = 0; i < pipeline.GetRenderingSRB()->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = pipeline.GetRenderingSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
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
							tex = &mParentAsset->mReflection.mShadowMapsInfo.mDirPos_SMInfo;
						}
						else if (VarName.find("Spot") == 0)
						{
							tex = &mParentAsset->mReflection.mShadowMapsInfo.mSpot_SMInfo;
						}
						else if (VarName.find("OmniDir") == 0)
						{
							tex = &mParentAsset->mReflection.mShadowMapsInfo.mOmniDir_SMInfo;
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
					else if (VarName.find("NEIBL_") == 0)
					{
						VarName.erase(0, 7);

						Assets::ShaderTexture ReflectedTex;
						ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
						ReflectedTex.mTex.SetName(VarName);
						ReflectedTex.mSlot = i;
						ReflectedTex.mType = Assets::ShaderTextureType::IBL_Tex;
						ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
						mParentAsset->mReflection.mIBLTexturesInfo.push_back(ReflectedTex);
					}
					else if (!pipeline.isDeffered())
					{
						if (VarName.find("NEMat_") == 0)
						{
							VarName.erase(0, 7);

							Assets::ShaderTexture ReflectedTex;
							ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
							ReflectedTex.mTex.SetName(VarName);
							ReflectedTex.mSlot = i;
							ReflectedTex.mType = Assets::ShaderTextureType::MaterialTex;
							ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
							mParentAsset->mReflection.mMaterialTexturesInfo.push_back(ReflectedTex);
						}
					}
				}
			}
		}
	}
}