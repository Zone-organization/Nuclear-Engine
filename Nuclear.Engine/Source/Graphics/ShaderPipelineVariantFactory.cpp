#include <Graphics/ShaderPipelineVariantFactory.h>
#include <Graphics/ShaderPipeline.h>
#include <Rendering/ImageBasedLighting.h>
#include <Fallbacks/FallbacksModule.h>
#include <Assets/Shader.h>
#include <Graphics/GraphicsModule.h>
#include <Platform/FileSystem.h>
#include <Rendering/RenderingModule.h>
#include <Utilities/Logger.h>
#include <Graphics/GraphicsModule.h>

namespace Nuclear
{
	namespace Graphics
	{
		using namespace Diligent;
		ShaderPipelineVariantFactory::ShaderPipelineVariantFactory()
		{
			mRenderQueueCounter = 1;
		}

		void SetSuitableLayout(InputLayoutDesc& result, ShaderType shadertype)
		{
			if (shadertype == ShaderType::Rendering3D)
			{
				result.LayoutElements = Graphics::GraphicsModule::Get().GetRendering3DInputLayout().data();
				result.NumElements = static_cast<Uint32>(Graphics::GraphicsModule::Get().GetRendering3DInputLayout().size());
			}
			else if (shadertype == ShaderType::PostFX || shadertype == ShaderType::RenderingEffect)
			{
				result.LayoutElements = Graphics::GraphicsModule::Get().GetRenderToTextureInputLayout().data();
				result.NumElements = static_cast<Uint32>(Graphics::GraphicsModule::Get().GetRenderToTextureInputLayout().size());
			}
			else
			{
				NUCLEAR_WARN("[ShaderPipelineVariantFactory] No suitable InputLayoutDesc found for the shader...");
			}
		}

		bool ShaderPipelineVariantFactory::CreateForwardVariant(ShaderPipelineVariant& variant, ShaderPipeline& pipeline, const ShaderPipelineBakingDesc& bakingdesc)
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

			GraphicsModule::Get().CreateShader(VShader.RawDblPtr(), variantdesc.mMainPSOCreateInfo.mVertexShader);
			GraphicsModule::Get().CreateShader(PShader.RawDblPtr(), variantdesc.mMainPSOCreateInfo.mPixelShader);

			GraphicsPipelineStateCreateInfo PSOCreateInfo;
			std::string psoname(Desc.mName + "_FID_" + std::to_string(variantdesc.mHashKey));
			variant.mName = psoname;
			PSOCreateInfo.PSODesc.Name = psoname.c_str();
			PSOCreateInfo.GraphicsPipeline = Desc.mForwardPSOCreateInfo.GraphicsPipeline;
			PSOCreateInfo.pVS = VShader;
			PSOCreateInfo.pPS = PShader;

			if (variantdesc.mMainPSOCreateInfo.mLayout.size() == 0)
			{
				SetSuitableLayout(PSOCreateInfo.GraphicsPipeline.InputLayout, pipeline.pParentShader->GetShaderBuildDesc().mType);
			}
			else
			{
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = variantdesc.mMainPSOCreateInfo.mLayout.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(variantdesc.mMainPSOCreateInfo.mLayout.size());
			}
			PSOResourcesInitInfo pso_resources = bakingdesc.mPipelineResourcesInfo;
			Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, pso_resources);

			if (!bakingdesc.mPreVariantPipelineCreationCallback.isNull())
				bakingdesc.mPreVariantPipelineCreationCallback(PSOCreateInfo);

			Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &variant.mPipeline);

			for (auto& i : bakingdesc.mStaticVariablesBindings)
			{
				SetIfFound(variant.mPipeline, i.mShaderStage, i.mName.c_str(), i.pObject);
			}

			variant.mPipeline->CreateShaderResourceBinding(&variant.mPipelineSRB, true);

			ReflectShaderPipelineVariant(variant);

			if (!bakingdesc.mPostVariantReflectionCallback.isNull())
				bakingdesc.mPostVariantReflectionCallback(variant.mReflection);

			return true;
		}

		bool ShaderPipelineVariantFactory::CreateDefferedVariant(ShaderPipelineVariant& variant, ShaderPipeline& pipeline, const ShaderPipelineBakingDesc& bakingdesc)
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

				GraphicsModule::Get().CreateShader(VSShader.RawDblPtr(), variant.mDesc.mGBufferPSOCreateInfo.mVertexShader);
				GraphicsModule::Get().CreateShader(PSShader.RawDblPtr(), variant.mDesc.mGBufferPSOCreateInfo.mPixelShader);

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline = Desc.mGBufferPSOCreateInfo.GraphicsPipeline;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = !COORDSYSTEM_LH_ENABLED;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

				if (variant.mDesc.mGBufferPSOCreateInfo.mLayout.size() == 0)
				{
					SetSuitableLayout(PSOCreateInfo.GraphicsPipeline.InputLayout, pipeline.pParentShader->GetShaderBuildDesc().mType);
				}
				else
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = variant.mDesc.mMainPSOCreateInfo.mLayout.data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(variant.mDesc.mMainPSOCreateInfo.mLayout.size());
				}
				PSOResourcesInitInfo pso_resources = bakingdesc.mPipelineResourcesInfo;
				Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, pso_resources);

				if (!bakingdesc.mPreVariantPipelineCreationCallback.isNull())
					bakingdesc.mPreVariantPipelineCreationCallback(PSOCreateInfo);

				Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &variant.mGBufferPipeline);


				for (auto& i : bakingdesc.mStaticVariablesBindings)
				{
					SetIfFound(variant.mGBufferPipeline, i.mShaderStage, i.mName.c_str(), i.pObject);
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

				GraphicsModule::Get().CreateShader(VSShader.RawDblPtr(), variant.mDesc.mMainPSOCreateInfo.mVertexShader);
				GraphicsModule::Get().CreateShader(PSShader.RawDblPtr(), variant.mDesc.mMainPSOCreateInfo.mPixelShader);
				PSOCreateInfo.PSODesc.Name = psoname.c_str();
				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline = Desc.mDefferedPSOCreateInfo.GraphicsPipeline;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = COORDSYSTEM_LH_ENABLED;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				if (PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements == 0)  //TODO: Move to shader parsing
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Graphics::GraphicsModule::Get().GetRenderToTextureInputLayout().data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Graphics::GraphicsModule::Get().GetRenderToTextureInputLayout().size());
				}
				else
				{
					PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = variant.mDesc.mMainPSOCreateInfo.mLayout.data();
					PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(variant.mDesc.mMainPSOCreateInfo.mLayout.size());
				}
				PSOResourcesInitInfo pso_resources = bakingdesc.mPipelineResourcesInfo;
				Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, pso_resources);

				if (!bakingdesc.mPreVariantPipelineCreationCallback.isNull())
					bakingdesc.mPreVariantPipelineCreationCallback(PSOCreateInfo);

				Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &variant.mPipeline);

				for (auto& i : bakingdesc.mStaticVariablesBindings)
				{
					SetIfFound(variant.mPipeline, i.mShaderStage, i.mName.c_str(), i.pObject);
				}

				variant.mPipeline->CreateShaderResourceBinding(&variant.mPipelineSRB, true);
			}
			ReflectShaderPipelineVariant(variant);

			if(!bakingdesc.mPostVariantReflectionCallback.isNull())
				bakingdesc.mPostVariantReflectionCallback(variant.mReflection);

			return true;
		}
		void ReflectVariantPSO(ShaderVariantReflection& result, Diligent::IShaderResourceBinding* SRB)
		{
			for (Uint32 i = 0; i < SRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = SRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
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
							tex = &result.mShadowMapsInfo.mDirPos_SMInfo;
						}
						else if (VarName.find("Spot") == 0)
						{
							tex = &result.mShadowMapsInfo.mSpot_SMInfo;
						}
						else if (VarName.find("OmniDir") == 0)
						{
							tex = &result.mShadowMapsInfo.mOmniDir_SMInfo;
						}
						else
						{
							assert(false);
						}
						tex->mTex.pTexture = Fallbacks::FallbacksModule::Get().GetDefaultWhiteImage();
						tex->mTex.mUsageType = GraphicsModule::Get().ParseTexUsageFromName(VarName);
						tex->mName = VarName;
						tex->mSlot = i;
						tex->mType = Assets::ShaderTextureType::ShadowTex;
					}
					else if (VarName.find("NEIBL_") == 0)
					{
						VarName.erase(0, 6);

						Assets::ShaderTexture ReflectedTex;
						ReflectedTex.mTex.pTexture = Fallbacks::FallbacksModule::Get().GetDefaultBlackImage();
						ReflectedTex.mTex.mUsageType = GraphicsModule::Get().ParseTexUsageFromName(VarName);
						ReflectedTex.mName = VarName;
						ReflectedTex.mSlot = i;
						ReflectedTex.mType = Assets::ShaderTextureType::RenderingEffect;
						result.mIBLTexturesInfo.push_back(ReflectedTex);
					}
					else if (VarName.find("NE_FX_") == 0)
					{
						VarName.erase(0, 6);
						Assets::ShaderTexture ReflectedTex;
						ReflectedTex.mTex.pTexture = Fallbacks::FallbacksModule::Get().GetDefaultBlackImage();
						ReflectedTex.mTex.mUsageType = GraphicsModule::Get().ParseTexUsageFromName(VarName);
						ReflectedTex.mName = VarName;
						ReflectedTex.mSlot = i;
						ReflectedTex.mType = Assets::ShaderTextureType::RenderingEffect;
						result.mRenderingEffects.push_back(ReflectedTex);
					}
				}
			}
		}
		void ShaderPipelineVariantFactory::ReflectShaderPipelineVariant(ShaderPipelineVariant& pipeline)
		{
			if (pipeline.mDesc.isDeffered)
			{
				return ReflectVariantPSO(pipeline.mReflection, pipeline.mPipelineSRB);
			}
			else
			{
				return ReflectVariantPSO(pipeline.mReflection, pipeline.GetRenderingSRB());
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