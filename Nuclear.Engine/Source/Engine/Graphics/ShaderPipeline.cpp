#include <Engine\Graphics\ShaderPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Core/Utilities/Hash.h>
#include <Core\Logger.h>
#include <Engine/Managers/AssetManager.h>
#include "Engine/Rendering/ImageBasedLighting.h"

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

		//std::string MergeCode(const std::string& shadersource, const std::set<std::string>& defines)
		//{
		//	std::vector<std::string> MergedCode;

		//	for (auto& i : defines)
		//	{
		//		MergedCode.push_back("#define " + i + "\n");
		//	}
		//	std::string str;
		//	MergedCode.push_back(shadersource);
		//	for (auto& i : MergedCode)
		//	{
		//		str = str + i;
		//	}
		//	return str;
		//}

		ShaderPipeline::ShaderPipeline(Assets::Shader* parent)
			: mParentAsset(parent)
		{
			mReflection = Graphics::ShaderReflection();
		}

		ShaderPipeline::~ShaderPipeline()
		{
			mVariants.clear();
		}

		void ShaderPipeline::Create(const ShaderPipelineDesc& Desc)
		{
			mDesc = Desc;
			ShaderPipelineVariantDesc ZeroInstance;
			ZeroInstance.mHashKey = 0;
			if (Desc.isDeffered)
			{
				ZeroInstance._isDeffered = true;
			}
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
							if (i == "NE_DEFFERED")
							{
								mReflection.mHasDefferedPipelines = true;
								Info_._isDeffered = true;
							}
							else if (i == "NE_ANIMATION")
							{
								Info_._isSkinned = true;
							}
							else if (i == "NE_SHADOWS")
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
			mDesc.pBakingDesc = bakingdesc;

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

			if (bakingdesc)
			{
				mGBuffer.Bake(Math::Vector2ui(bakingdesc->mRTWidth, bakingdesc->mRTHeight));
			}

			return true;
		}

		void SetIfFound(IPipelineState* pipeline, SHADER_TYPE ShaderType, const Char* Name, IDeviceObject* obj)
		{
			auto res = pipeline->GetStaticVariableByName(ShaderType, Name);

			if (res)
			{
				if (obj)
				{
					res->Set(obj);
				}
			}
		}
		void AddToDefinesIfNotZero(std::set<std::string>& defines, const std::string& name, Uint32 value)
		{
			if (value > 0) { defines.insert(name + std::to_string(value)); }
		}
		ShaderPipelineVariant ShaderPipeline::CreateForwardVariant(ShaderPipelineVariantDesc& Info, ShaderPipelineDesc& Desc)
		{
			ShaderPipelineVariant result;
			result.pParent = this;
			if (mParentAsset)
			{
				result.mShaderAssetID = mParentAsset->GetID();
			}
			result.mDesc = Info;
			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;

			if (Desc.pBakingDesc)
			{
				AddToDefinesIfNotZero(Info.mDefines, "NE_DIR_LIGHTS_NUM ", Desc.pBakingDesc->DirLights);
				AddToDefinesIfNotZero(Info.mDefines, "NE_SPOT_LIGHTS_NUM ", Desc.pBakingDesc->SpotLights);
				AddToDefinesIfNotZero(Info.mDefines, "NE_POINT_LIGHTS_NUM ", Desc.pBakingDesc->PointLights);

				if (Desc.pBakingDesc->pShadowPass)
				{
					AddToDefinesIfNotZero(Info.mDefines, "NE_MAX_DIR_CASTERS ", Desc.pBakingDesc->pShadowPass->GetBakingDesc().MAX_DIR_CASTERS);
					AddToDefinesIfNotZero(Info.mDefines, "NE_MAX_SPOT_CASTERS ", Desc.pBakingDesc->pShadowPass->GetBakingDesc().MAX_SPOT_CASTERS);
					AddToDefinesIfNotZero(Info.mDefines, "NE_MAX_OMNIDIR_CASTERS ", Desc.pBakingDesc->pShadowPass->GetBakingDesc().MAX_OMNIDIR_CASTERS);
				}
			}
			Info.mMainPSOCreateInfo = Desc.mForwardPSOCreateInfo;

			for (auto& i : Info.mDefines)
			{
				Info.mMainPSOCreateInfo.mVertexShader.mDefines.insert(i);
				Info.mMainPSOCreateInfo.mPixelShader.mDefines.insert(i);
			}
			GraphicsEngine::GetShaderManager()->CreateShader(VShader.RawDblPtr(), Info.mMainPSOCreateInfo.mVertexShader);
			GraphicsEngine::GetShaderManager()->CreateShader(PShader.RawDblPtr(), Info.mMainPSOCreateInfo.mPixelShader);

			GraphicsPipelineStateCreateInfo PSOCreateInfo;
			std::string psoname(Desc.mName + "_FID_" + std::to_string(Info.mHashKey));
			result.mName = psoname;
			PSOCreateInfo.PSODesc.Name = psoname.c_str();
			PSOCreateInfo.GraphicsPipeline = Desc.mForwardPSOCreateInfo.GraphicsPipeline;
			PSOCreateInfo.pVS = VShader;
			PSOCreateInfo.pPS = PShader;

			std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetShaderManager()->GetBasicVSLayout(false);
			if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
			{
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			}
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&result.mPipeline, PSOCreateInfo, Vars, true);

			if (Desc.pBakingDesc)
			{
				SetIfFound(result.mPipeline, SHADER_TYPE_VERTEX, "NEStatic_Camera", Desc.pBakingDesc->CameraBufferPtr);
				SetIfFound(result.mPipeline, SHADER_TYPE_VERTEX, "NEStatic_Animation", Desc.pBakingDesc->AnimationBufferPtr);
				SetIfFound(result.mPipeline, SHADER_TYPE_PIXEL, "NEStatic_Lights", Desc.pBakingDesc->LightsBufferPtr);
			}

			result.mPipeline->CreateShaderResourceBinding(&result.mPipelineSRB, true);

			ReflectShaderPipelineVariant(result, Desc.pBakingDesc);
			return result;
		}

		ShaderPipelineVariant ShaderPipeline::CreateDefferedVariant(ShaderPipelineVariantDesc& Info, ShaderPipelineDesc& Desc)
		{
			ShaderPipelineVariant result;
			result.pParent = this;
			if (mParentAsset)
			{
				result.mShaderAssetID = mParentAsset->GetID();
			}
			result.mDesc = Info;
			Info.mGBufferPSOCreateInfo = Desc.mGBufferPSOCreateInfo;
			Info.mMainPSOCreateInfo = Desc.mDefferedPSOCreateInfo;

			//Geometry -GBuffer- Pass
			{

				//Pipeline
				GraphicsPipelineStateCreateInfo PSOCreateInfo;
				std::string psoname(Desc.mName + "_GBID_" + std::to_string(Info.mHashKey));
				PSOCreateInfo.PSODesc.Name = psoname.c_str();

				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;
				if (Desc.pBakingDesc)
				{
					AddToDefinesIfNotZero(Info.mDefines, "NE_DIR_LIGHTS_NUM ", Desc.pBakingDesc->DirLights);
					AddToDefinesIfNotZero(Info.mDefines, "NE_SPOT_LIGHTS_NUM ", Desc.pBakingDesc->SpotLights);
					AddToDefinesIfNotZero(Info.mDefines, "NE_POINT_LIGHTS_NUM ", Desc.pBakingDesc->PointLights);

					if (Desc.pBakingDesc->pShadowPass)
					{
						AddToDefinesIfNotZero(Info.mDefines, "NE_MAX_DIR_CASTERS ", Desc.pBakingDesc->pShadowPass->GetBakingDesc().MAX_DIR_CASTERS);
						AddToDefinesIfNotZero(Info.mDefines, "NE_MAX_SPOT_CASTERS ", Desc.pBakingDesc->pShadowPass->GetBakingDesc().MAX_SPOT_CASTERS);
						AddToDefinesIfNotZero(Info.mDefines, "NE_MAX_OMNIDIR_CASTERS ", Desc.pBakingDesc->pShadowPass->GetBakingDesc().MAX_OMNIDIR_CASTERS);
					}
				}
				for (auto& i : Info.mDefines)
				{
					
					Info.mGBufferPSOCreateInfo.mVertexShader.mDefines.insert(i);
					Info.mGBufferPSOCreateInfo.mPixelShader.mDefines.insert(i);
				}
				GraphicsEngine::GetShaderManager()->CreateShader(VSShader.RawDblPtr(), Info.mGBufferPSOCreateInfo.mVertexShader);
				GraphicsEngine::GetShaderManager()->CreateShader(PSShader.RawDblPtr(), Info.mGBufferPSOCreateInfo.mPixelShader);

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline = Desc.mGBufferPSOCreateInfo.GraphicsPipeline;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = !COORDSYSTEM_LH_ENABLED;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

				std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetShaderManager()->GetBasicVSLayout(false);
				if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
				}

				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&result.mGBufferPipeline, PSOCreateInfo, Vars, true);


				if (Desc.pBakingDesc)
				{
					SetIfFound(result.mGBufferPipeline, SHADER_TYPE_VERTEX, "NEStatic_Camera", Desc.pBakingDesc->CameraBufferPtr);

					SetIfFound(result.mGBufferPipeline, SHADER_TYPE_PIXEL, "NEStatic_Lights", Desc.pBakingDesc->LightsBufferPtr);
				}
				result.mGBufferPipeline->CreateShaderResourceBinding(&result.mGBufferSRB, true);

				//Create GBuffer Desc
				if (!mGBuffer.isInitialized())
				{
					Rendering::GBufferDesc gbufferdesc;

					for (Uint32 i = 0; i < PSOCreateInfo.GraphicsPipeline.NumRenderTargets; i++)
					{
						Graphics::RenderTargetDesc RTDesc;
						RTDesc.ColorTexFormat = PSOCreateInfo.GraphicsPipeline.RTVFormats[i];
						RTDesc.mName = Info.mGBufferPSOCreateInfo.mRTsNames.at(i);
						gbufferdesc.mRTDescs.push_back(RTDesc);
					}
					mGBuffer.Initialize(gbufferdesc);
				}
			}
			//Deffered -Lighting- Pass
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;
				std::string psoname(Desc.mName + "_DID_" + std::to_string(Info.mHashKey));
				result.mName = psoname;

				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;

				for (auto& i : Info.mDefines)
				{
					Info.mMainPSOCreateInfo.mVertexShader.mDefines.insert(i);
					Info.mMainPSOCreateInfo.mPixelShader.mDefines.insert(i);
				}
				GraphicsEngine::GetShaderManager()->CreateShader(VSShader.RawDblPtr(), Info.mMainPSOCreateInfo.mVertexShader);
				GraphicsEngine::GetShaderManager()->CreateShader(PSShader.RawDblPtr(), Info.mMainPSOCreateInfo.mPixelShader);
				PSOCreateInfo.PSODesc.Name = psoname.c_str();
				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline = Desc.mDefferedPSOCreateInfo.GraphicsPipeline;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = COORDSYSTEM_LH_ENABLED;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = FALSE;

				std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetShaderManager()->GetBasicVSLayout(true);
				if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
				}

				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&result.mPipeline, PSOCreateInfo, Vars, true);

				if (Desc.pBakingDesc)
				{
					SetIfFound(result.mPipeline, SHADER_TYPE_VERTEX, "NEStatic_Camera", Desc.pBakingDesc->CameraBufferPtr);
					SetIfFound(result.mPipeline, SHADER_TYPE_VERTEX, "NEStatic_Animation", Desc.pBakingDesc->AnimationBufferPtr);
					SetIfFound(result.mPipeline, SHADER_TYPE_PIXEL, "NEStatic_Lights", Desc.pBakingDesc->LightsBufferPtr);

				}

				result.mPipeline->CreateShaderResourceBinding(&result.mPipelineSRB, true);
			}
			ReflectShaderPipelineVariant(result, Desc.pBakingDesc);

			return result;
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

			if (mVariants.size() > 0)
				return &mVariants.begin()->second;

			return nullptr;
		}

		const std::vector<ShaderPipelineSwitch>& ShaderPipeline::GetSwitches() const
		{
			return mDesc.Switches;
		}

		const Graphics::ShaderReflection& ShaderPipeline::GetReflection() const
		{
			return mReflection;
		}

		Assets::Shader* ShaderPipeline::GetShaderAsset()
		{
			return mParentAsset;
		}

		Rendering::GBuffer* ShaderPipeline::GetGBuffer()
		{
			return &mGBuffer;
		}

		bool ShaderPipeline::GetAlwaysRequestDeffered()
		{
			return mAlwaysRequestDeffered;
		}

		void ShaderPipeline::ReflectShaderPipelineVariant(ShaderPipelineVariant& pipeline, ShaderRenderingBakingDesc* pBakingDesc)
		{
			if (pipeline.isDeffered())
			{
				if (mFirstReflection)
				{
					//Gbuffer Reflection
					for (Uint32 i = 0; i < pipeline.GetGBufferPipelineSRB()->GetVariableCount(SHADER_TYPE_PIXEL); i++)
					{
						auto variable = pipeline.GetGBufferPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
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
							ReflectedTex.mType = Assets::ShaderTextureType::MaterialTex;
							ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
							mReflection.mMaterialTexturesInfo.push_back(ReflectedTex);
						}
					}
				}

				//Main pipeline Reflection
				for (Uint32 i = 0; i < pipeline.GetMainPipelineSRB()->GetVariableCount(SHADER_TYPE_PIXEL); i++)
				{
					auto variable = pipeline.GetMainPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
					ShaderResourceDesc VarDesc;
					variable->GetResourceDesc(VarDesc);
					std::string VarName(VarDesc.Name);
					auto VarType = VarDesc.Type;
					if (VarType == SHADER_RESOURCE_TYPE_TEXTURE_SRV)
					{
						//Shadow Maps
						if (VarName.find("NE_ShadowMap_") == 0)
						{
							VarName.erase(0, 13);

							Assets::ShaderTexture* tex;
							if (VarName.find("DirPos") == 0)
							{
								tex = &pipeline.mReflection.mShadowMapsInfo.mDirPos_SMInfo;
							}
							else if (VarName.find("Spot") == 0)
							{
								tex = &pipeline.mReflection.mShadowMapsInfo.mSpot_SMInfo;
							}
							else if (VarName.find("OmniDir") == 0)
							{
								tex = &pipeline.mReflection.mShadowMapsInfo.mOmniDir_SMInfo;
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
							VarName.erase(0, 6);

							Assets::ShaderTexture ReflectedTex;
							ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
							ReflectedTex.mTex.SetName(VarName);
							ReflectedTex.mSlot = i;
							ReflectedTex.mType = Assets::ShaderTextureType::IBL_Tex;
							ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
							pipeline.mReflection.mIBLTexturesInfo.push_back(ReflectedTex);
						}
					}
				}
			}
			else
			{
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
						if (VarName.find("NE_ShadowMap_") == 0)
						{
							VarName.erase(0, 13);

							Assets::ShaderTexture* tex;
							if (VarName.find("DirPos") == 0)
							{
								tex = &pipeline.mReflection.mShadowMapsInfo.mDirPos_SMInfo;
							}
							else if (VarName.find("Spot") == 0)
							{
								tex = &pipeline.mReflection.mShadowMapsInfo.mSpot_SMInfo;
							}
							else if (VarName.find("OmniDir") == 0)
							{
								tex = &pipeline.mReflection.mShadowMapsInfo.mOmniDir_SMInfo;
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
							VarName.erase(0, 6);

							Assets::ShaderTexture ReflectedTex;
							ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
							ReflectedTex.mTex.SetName(VarName);
							ReflectedTex.mSlot = i;
							ReflectedTex.mType = Assets::ShaderTextureType::IBL_Tex;
							ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
							pipeline.mReflection.mIBLTexturesInfo.push_back(ReflectedTex);
						}
						else if (mFirstReflection)
						{
							if (VarName.find("NEMat_") == 0)
							{
								VarName.erase(0, 6);

								Assets::ShaderTexture ReflectedTex;
								ReflectedTex.mTex = Managers::AssetManager::DefaultBlackTex;
								ReflectedTex.mTex.SetName(VarName);
								ReflectedTex.mSlot = i;
								ReflectedTex.mType = Assets::ShaderTextureType::MaterialTex;
								ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
								mReflection.mMaterialTexturesInfo.push_back(ReflectedTex);
							}
						}
					}
				}
			}

			if (pBakingDesc)
			{
				if (pBakingDesc->pIBLContext)
				{
					for (auto& i : pipeline.mReflection.mIBLTexturesInfo)
					{
						if (i.mTex.GetUsageType() == Graphics::TextureUsageType::IrradianceMap)
						{
							i.mTex.SetImage(&pBakingDesc->pIBLContext->GetEnvironmentCapture()->mIrradiance);
						}
						else if (i.mTex.GetUsageType() == Graphics::TextureUsageType::PreFilterMap)
						{
							i.mTex.SetImage(&pBakingDesc->pIBLContext->GetEnvironmentCapture()->mPrefiltered);
						}
						else if (i.mTex.GetUsageType() == Graphics::TextureUsageType::BRDF_LUT)
						{
							i.mTex.SetImage(pBakingDesc->pIBLContext->GetBRDF_LUT());
						}
					}
				}
			}

			mFirstReflection = false;
		}
	}
}