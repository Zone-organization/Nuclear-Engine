#include <Graphics/ShaderPipelineVariantFactory.h>
#include <Graphics/ShaderPipeline.h>
#include <Rendering/ImageBasedLighting.h>
#include <Fallbacks/FallbacksEngine.h>
#include <Assets/Shader.h>
#include <Graphics/GraphicsEngine.h>
#include <Platform/FileSystem.h>
#include <Rendering/RenderingEngine.h>

namespace Nuclear
{
	namespace Graphics
	{
		using namespace Diligent;
		ShaderPipelineVariantFactory::ShaderPipelineVariantFactory()
		{
			mRenderQueueCounter = 1;
		}

		bool ShaderPipelineVariantFactory::CreateForwardVariant(ShaderPipelineVariant& variant, ShaderPipeline& pipeline, ShaderPipelineBakingDesc& bakingdesc)
		{
			variant.mRenderQueue = mRenderQueueCounter;
			mRenderQueueCounter++;

			ShaderPipelineDesc& Desc = pipeline.mDesc;
			ShaderPipelineVariantDesc& variantdesc = variant.mDesc;

			variant.pParent = &pipeline;
			if (pipeline.pParentShader)
			{
				variant.mShaderAssetID = pipeline.pParentShader->GetID();
			}
			variantdesc.mMainPSOCreateInfo = Desc.mForwardPSOCreateInfo;

			for (auto& i : variantdesc.mDefines)
			{
				variantdesc.mMainPSOCreateInfo.mVertexShader.mDefines.insert(i);
				variantdesc.mMainPSOCreateInfo.mPixelShader.mDefines.insert(i);
			}
			for (auto& i : bakingdesc.mDefines)
			{
				variantdesc.mMainPSOCreateInfo.mVertexShader.mDefines.insert(i);
				variantdesc.mMainPSOCreateInfo.mPixelShader.mDefines.insert(i);
			}

			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;

			GraphicsEngine::GetInstance().CreateShader(VShader.RawDblPtr(), variantdesc.mMainPSOCreateInfo.mVertexShader);
			GraphicsEngine::GetInstance().CreateShader(PShader.RawDblPtr(), variantdesc.mMainPSOCreateInfo.mPixelShader);

			GraphicsPipelineStateCreateInfo PSOCreateInfo;
			std::string psoname(Desc.mName + "_FID_" + std::to_string(variantdesc.mHashKey));
			variant.mName = psoname;
			PSOCreateInfo.PSODesc.Name = psoname.c_str();
			PSOCreateInfo.GraphicsPipeline = Desc.mForwardPSOCreateInfo.GraphicsPipeline;
			PSOCreateInfo.pVS = VShader;
			PSOCreateInfo.pPS = PShader;

			std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetInstance().GetBasicVSLayout(false);
			if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
			{
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			}
			auto Vars = Graphics::GraphicsEngine::GetInstance().ReflectShaderVariables(VShader, PShader);
			Graphics::GraphicsEngine::GetInstance().ProcessAndCreatePipeline(&variant.mPipeline, PSOCreateInfo, Vars, true);

			for (auto& i : bakingdesc.mCBsBindings)
			{
				SetIfFound(variant.mPipeline, i.mShaderStage, i.mName.c_str(), i.pCB);
			}

			variant.mPipeline->CreateShaderResourceBinding(&variant.mPipelineSRB, true);

			ReflectShaderPipelineVariant(variant);

			if (!bakingdesc.mPostVariantReflectionCallback.isNull())
				bakingdesc.mPostVariantReflectionCallback(variant.mReflection);

			return true;
		}

		bool ShaderPipelineVariantFactory::CreateDefferedVariant(ShaderPipelineVariant& variant, ShaderPipeline& pipeline, ShaderPipelineBakingDesc& bakingdesc)
		{
			variant.mRenderQueue = mRenderQueueCounter;
			mRenderQueueCounter++;

			ShaderPipelineDesc& Desc = pipeline.mDesc;
			variant.pParent = &pipeline;
			if (pipeline.pParentShader)
			{
				variant.mShaderAssetID = pipeline.pParentShader->GetID();
			}

			variant.mDesc.mGBufferPSOCreateInfo = Desc.mGBufferPSOCreateInfo;
			variant.mDesc.mMainPSOCreateInfo = Desc.mDefferedPSOCreateInfo;

			//Geometry -GBuffer- Pass
			if (variant.mDesc.mGBufferPSOCreateInfo.mValid)
			{
				//Pipeline
				GraphicsPipelineStateCreateInfo PSOCreateInfo;
				std::string psoname(Desc.mName + "_GBID_" + std::to_string(variant.mDesc.mHashKey));
				PSOCreateInfo.PSODesc.Name = psoname.c_str();
			
				for (auto& i : variant.mDesc.mDefines)
				{
					variant.mDesc.mGBufferPSOCreateInfo.mVertexShader.mDefines.insert(i);
					variant.mDesc.mGBufferPSOCreateInfo.mPixelShader.mDefines.insert(i);
				}
				for (auto& i : bakingdesc.mDefines)
				{
					variant.mDesc.mGBufferPSOCreateInfo.mVertexShader.mDefines.insert(i);
					variant.mDesc.mGBufferPSOCreateInfo.mPixelShader.mDefines.insert(i);
				}

				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;

				GraphicsEngine::GetInstance().CreateShader(VSShader.RawDblPtr(), variant.mDesc.mGBufferPSOCreateInfo.mVertexShader);
				GraphicsEngine::GetInstance().CreateShader(PSShader.RawDblPtr(), variant.mDesc.mGBufferPSOCreateInfo.mPixelShader);

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline = Desc.mGBufferPSOCreateInfo.GraphicsPipeline;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = !COORDSYSTEM_LH_ENABLED;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

				std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetInstance().GetBasicVSLayout(false);
				if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
				}

				auto Vars = Graphics::GraphicsEngine::GetInstance().ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetInstance().ProcessAndCreatePipeline(&variant.mGBufferPipeline, PSOCreateInfo, Vars, true);

				for (auto& i : bakingdesc.mCBsBindings)
				{
					SetIfFound(variant.mGBufferPipeline, i.mShaderStage, i.mName.c_str(), i.pCB);
				}

				variant.mGBufferPipeline->CreateShaderResourceBinding(&variant.mGBufferSRB, true);

				//Create GBuffer Desc
				if (!pipeline.mGBuffer.isInitialized())
				{
					Rendering::GBufferDesc gbufferdesc;

					for (Uint32 i = 0; i < PSOCreateInfo.GraphicsPipeline.NumRenderTargets; i++)
					{
						Graphics::RenderTargetDesc RTDesc;
						RTDesc.ColorTexFormat = PSOCreateInfo.GraphicsPipeline.RTVFormats[i];
						RTDesc.mName = variant.mDesc.mGBufferPSOCreateInfo.mRTsNames.at(i);
						gbufferdesc.mRTDescs.push_back(RTDesc);
					}
					pipeline.mGBuffer.Initialize(gbufferdesc);
				}
			}
			//Deffered -Lighting- Pass
			if (variant.mDesc.mMainPSOCreateInfo.mValid)
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;
				std::string psoname(Desc.mName + "_DID_" + std::to_string(variant.mDesc.mHashKey));
				variant.mName = psoname;

				for (auto& i : variant.mDesc.mDefines)
				{
					variant.mDesc.mMainPSOCreateInfo.mVertexShader.mDefines.insert(i);
					variant.mDesc.mMainPSOCreateInfo.mPixelShader.mDefines.insert(i);
				}
				for (auto& i : bakingdesc.mDefines)
				{
					variant.mDesc.mMainPSOCreateInfo.mVertexShader.mDefines.insert(i);
					variant.mDesc.mMainPSOCreateInfo.mPixelShader.mDefines.insert(i);
				}

				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;

				GraphicsEngine::GetInstance().CreateShader(VSShader.RawDblPtr(), variant.mDesc.mMainPSOCreateInfo.mVertexShader);
				GraphicsEngine::GetInstance().CreateShader(PSShader.RawDblPtr(), variant.mDesc.mMainPSOCreateInfo.mPixelShader);
				PSOCreateInfo.PSODesc.Name = psoname.c_str();
				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline = Desc.mDefferedPSOCreateInfo.GraphicsPipeline;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = COORDSYSTEM_LH_ENABLED;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetInstance().GetBasicVSLayout(true);
				if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
				}

				auto Vars = Graphics::GraphicsEngine::GetInstance().ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetInstance().ProcessAndCreatePipeline(&variant.mPipeline, PSOCreateInfo, Vars, true);

				for (auto& i : bakingdesc.mCBsBindings)
				{
					SetIfFound(variant.mPipeline, i.mShaderStage, i.mName.c_str(), i.pCB);
				}

				variant.mPipeline->CreateShaderResourceBinding(&variant.mPipelineSRB, true);
			}
			ReflectShaderPipelineVariant(variant);

			if(!bakingdesc.mPostVariantReflectionCallback.isNull())
				bakingdesc.mPostVariantReflectionCallback(variant.mReflection);

			return true;
		}

		void ShaderPipelineVariantFactory::ReflectShaderPipelineVariant(ShaderPipelineVariant& pipeline)
		{
			if (pipeline.mDesc.isDeffered)
			{
				//Main pipeline Reflection
				for (Uint32 i = 0; i < pipeline.mPipelineSRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
				{
					auto variable = pipeline.mPipelineSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
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
							else
							{
								assert(false);
							}
							tex->mTex.pTexture = Fallbacks::FallbacksEngine::GetInstance().GetDefaultWhiteImage();
							tex->mTex.mUsageType = GraphicsEngine::GetInstance().ParseTexUsageFromName(VarName);
							tex->mName = VarName;
							tex->mSlot = i;
							tex->mType = Assets::ShaderTextureType::ShadowTex;
						}
						else if (VarName.find("NEIBL_") == 0)
						{
							VarName.erase(0, 6);

							Assets::ShaderTexture ReflectedTex;
							ReflectedTex.mTex.pTexture = Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
							ReflectedTex.mTex.mUsageType = GraphicsEngine::GetInstance().ParseTexUsageFromName(VarName);
							ReflectedTex.mName = VarName;
							ReflectedTex.mSlot = i;
							ReflectedTex.mType = Assets::ShaderTextureType::IBL_Tex;
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
							else
							{
								assert(false);
							}
							tex->mTex.pTexture = Fallbacks::FallbacksEngine::GetInstance().GetDefaultWhiteImage();
							tex->mTex.mUsageType = GraphicsEngine::GetInstance().ParseTexUsageFromName(VarName);
							tex->mName = VarName;
							tex->mSlot = i;
							tex->mType = Assets::ShaderTextureType::ShadowTex;
						}
						else if (VarName.find("NEIBL_") == 0)
						{
							VarName.erase(0, 6);

							Assets::ShaderTexture ReflectedTex;
							ReflectedTex.mTex.pTexture = Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
							ReflectedTex.mTex.mUsageType = GraphicsEngine::GetInstance().ParseTexUsageFromName(VarName);
							ReflectedTex.mName = VarName;
							ReflectedTex.mSlot = i;
							ReflectedTex.mType = Assets::ShaderTextureType::IBL_Tex;
							pipeline.mReflection.mIBLTexturesInfo.push_back(ReflectedTex);
						}
					}
				}
			}
		}

		void ShaderPipelineVariantFactory::SetIfFound(IPipelineState* pipeline, Diligent::SHADER_TYPE ShaderType, const Char* Name, IDeviceObject* obj)
		{
			if (obj)
			{
				auto res = pipeline->GetStaticVariableByName(ShaderType, Name);
				if (res)
					res->Set(obj);
			}
		}
	}
}