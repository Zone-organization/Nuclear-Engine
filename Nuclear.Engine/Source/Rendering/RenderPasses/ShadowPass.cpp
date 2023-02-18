#include "Rendering\RenderPasses\ShadowPass.h"
#include <Platform\FileSystem.h>
#include <Graphics\Context.h>
#include <Utilities/Logger.h>
#include <Components/MeshComponent.h>
#include <Assets/Mesh.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Components\EntityInfoComponent.h>
#include <Core\Scene.h>
#include <Rendering/FrameRenderData.h>
#include <Math\Math.h>

namespace Nuclear
{
	namespace Rendering
	{
		using namespace Diligent;

		struct NEStatic_LightInfo {
			Math::Matrix4 Model;
			Math::Matrix4 LightSpace;
		};

		ShadowPass::ShadowPass()
		{
		}

		void ShadowPass::Bake(const ShadowPassBakingDesc& desc)
		{
			mDesc = desc;
			if (mDesc.MAX_DIR_CASTERS > 0 || mDesc.MAX_SPOT_CASTERS > 0)
			{
				InitPositionalShadowPassPSO();
				InitPositionalShadowMapTextures();

				BufferDesc CBDesc;
				CBDesc.Name = "NEStatic_LightSpaces";
				CBDesc.Size = (sizeof(Math::Matrix4) * mDesc.MAX_DIR_CASTERS) + (sizeof(Math::Matrix4) * mDesc.MAX_SPOT_CASTERS);
				CBDesc.Usage = USAGE_DYNAMIC;
				CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
				CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
				Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pLightSpacesCB);
			}

			if (mDesc.MAX_OMNIDIR_CASTERS > 0)
			{
				InitOmniDirShadowPassPSO();
				InitOmniDirShadowMapTexture();
			}
		}
		void ShadowPass::Update(FrameRenderData* framedata)
		{
			std::vector<Math::Matrix4> lightspacematrices;
			Uint32 DirPosCasterRTIndex = 0, SpotCasterRTIndex = 0, PointCasterRTIndex = 0;

			for (auto DirLight : framedata->DirLights)
			{
				if (DirLight->mCastShadows)
				{
					float ab = 10.f;
					auto lightProjection = glm::ortho(-ab, ab, -ab, ab, DirLight->GetNearPlane(), DirLight->GetFarPlane());
					auto lightView = glm::lookAt(DirLight->GetDirection(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

					DirLight->LightSpace = lightProjection * lightView;
					lightspacematrices.push_back(DirLight->LightSpace);

					DirLightShadowDepthPass(DirLight, DirPosCasterRTIndex);
					DirPosCasterRTIndex++;
				}
				else {
					break; //since they are ordered in rendersystem
				}
			}

			for (auto SpotLight : framedata->SpotLights)
			{
				if (SpotLight->mCastShadows)
				{
					auto lightProjection = glm::perspective(Math::radians(SpotLight->GetFOV()), 1024.f / 1024.f, SpotLight->GetNearPlane(), SpotLight->GetFarPlane());
					auto lightpos = SpotLight->GetInternalPosition();
					auto up = glm::vec3(0.0, 1.0, 0.0);

					auto lightView = glm::lookAt(lightpos, lightpos + SpotLight->GetDirection(), up);

					SpotLight->LightSpace = lightProjection * lightView;
					lightspacematrices.push_back(SpotLight->LightSpace);

					SpotLightShadowDepthPass(SpotLight, SpotCasterRTIndex);
					SpotCasterRTIndex++;
				}
				else {
					break; //since they are ordered in rendersystem
				}

			}

			for (auto PointLight : framedata->PointLights)
			{
				if (PointLight->mCastShadows)
				{
					PointLightShadowDepthPass(PointLight, PointCasterRTIndex);
					PointCasterRTIndex++;
				}
				else {
					break; //since they are ordered in rendersystem
				}
			}
		}

		void ShadowPass::DirLightShadowDepthPass(Components::LightComponent* light, Uint32 RTindex)
		{
		//	if (light.GetShadowType() == Components::LightShadowType::Simple_Shadows)
			{
				PositionalLightShadowDepthPass(RTindex, light->LightSpace, mDirShadowMap);
			}

			//CSM - WIP
		}

		void ShadowPass::SpotLightShadowDepthPass(Components::LightComponent* spotlight, Uint32 RTindex)
		{
			return PositionalLightShadowDepthPass(RTindex, spotlight->LightSpace, mSpotShadowMap);
		}

		void ShadowPass::PointLightShadowDepthPass(Components::LightComponent* pointlight, Uint32 RTindex)
		{
			Graphics::Context::GetInstance().GetContext()->SetPipelineState(mOmniDirShadowPassPSO.RawPtr());

			Graphics::Context::GetInstance().GetContext()->SetRenderTargets(0, nullptr, pOmniDirShadowMapDSVs[RTindex], RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetInstance().GetContext()->ClearDepthStencil(pOmniDirShadowMapDSVs[RTindex], CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetInstance().GetContext()->CommitShaderResources(mOmniDirShadowPassSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			auto view = Core::Scene::GetInstance().GetRegistry().view<Components::MeshComponent>();
			for (auto entity : view)
			{
				auto& MeshObject = view.get<Components::MeshComponent>(entity);
				if (MeshObject.GetCastShadow())               //TODO Animation component
				{
					auto& EntityInfo = Core::Scene::GetInstance().GetRegistry().get<Components::EntityInfoComponent>(entity);
					EntityInfo.mTransform.Update();

					auto lightPos = pointlight->GetInternalPosition();

					//Update cbuffer NEStatic_PointShadowVS	{	matrix Model;	};
					{
						Diligent::MapHelper<Math::Matrix4> CBConstants(Graphics::Context::GetInstance().GetContext(), pOmniDirShadowVS_CB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = EntityInfo.mTransform.GetWorldMatrix();
					}
					//Update cbuffer NEStatic_PointShadowGS	{matrix ShadowMatrices[6];	};
					{

						glm::mat4 shadowProj = glm::perspective(glm::radians(pointlight->GetFOV()), (float)1024 / (float)1024, pointlight->GetNearPlane(), pointlight->GetFarPlane());

						std::array<glm::mat4, 6> shadowTransforms = {
						shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),  //right
						shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),  //left
						shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),    //up
						shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),  //down
						shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),   //front
						shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))  //back
						};


						PVoid data;
						Graphics::Context::GetInstance().GetContext()->MapBuffer(pOmniDirShadowGS_CB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
						data = memcpy(data, shadowTransforms.data(), sizeof(Math::Matrix4) * 6);
						Graphics::Context::GetInstance().GetContext()->UnmapBuffer(pOmniDirShadowGS_CB, MAP_WRITE);
					}
				//	Update cbuffer NEStatic_PointShadowPS	{    float3 gLightPos;	float gFarPlane;	};
				    {
						Diligent::MapHelper<Math::vec4> CBConstants(Graphics::Context::GetInstance().GetContext(), pOmniDirShadowPS_CB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = glm::vec4(lightPos, pointlight->GetFarPlane());
					}
					RenderMeshForDepthPass(MeshObject.GetMesh());
				}
			}
		}

		ShadowPassBakingDesc ShadowPass::GetBakingDesc() const
		{
			return mDesc;
		}

		IBuffer* ShadowPass::GetLightSpacesCB()
		{
			return pLightSpacesCB.RawPtr();
		}

		ITextureView* ShadowPass::GetDirPosShadowMapSRV()
		{
			return mDirShadowMap.pPosShadowMapSRV.RawPtr();
		}
		ITextureView* ShadowPass::GetSpotShadowMapSRV()
		{
			return mSpotShadowMap.pPosShadowMapSRV.RawPtr();
		}
		ITextureView* ShadowPass::GetOmniDirShadowMapSRV()
		{
			return pOmniDirShadowMapSRV.RawPtr();
		}
		void ShadowPass::PositionalLightShadowDepthPass(Uint32 RTindex, const Math::Matrix4 lightspace, PosShadowMap& type)
		{
			Graphics::Context::GetInstance().GetContext()->SetPipelineState(mPositionalShadowMapDepthPSO.RawPtr());

			Graphics::Context::GetInstance().GetContext()->SetRenderTargets(0, nullptr, type.pPosShadowMapDSVs[RTindex], RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetInstance().GetContext()->ClearDepthStencil(type.pPosShadowMapDSVs[RTindex], CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetInstance().GetContext()->CommitShaderResources(mPositionalShadowMapDepthSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			auto view = Core::Scene::GetInstance().GetRegistry().view<Components::MeshComponent>();
			for (auto entity : view)
			{
				auto& MeshObject = view.get<Components::MeshComponent>(entity);
				if (MeshObject.GetCastShadow())               //TODO Animation component
				{
					auto& EntityInfo = Core::Scene::GetInstance().GetRegistry().get<Components::EntityInfoComponent>(entity);
					EntityInfo.mTransform.Update();

					NEStatic_LightInfo cbdata;
					cbdata.Model = EntityInfo.mTransform.GetWorldMatrix();
					cbdata.LightSpace = lightspace;
					{
						Diligent::MapHelper<NEStatic_LightInfo> CBConstants(Graphics::Context::GetInstance().GetContext(), pPositionalLightInfoCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = cbdata;
					}

					RenderMeshForDepthPass(MeshObject.GetMesh());
				}
			}
		}
		void ShadowPass::InitPositionalShadowPassPSO()
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "PositionalShadowMapDepthPass PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 0;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_D32_FLOAT;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = false;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.StencilEnable = false;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthBias = 8500; //maybe as parameter
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthBiasClamp = 0.0f;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.SlopeScaledDepthBias = 1.0f;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthClipEnable = true;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			std::vector<LayoutElement> LayoutElems;

			LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
			LayoutElems.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
			LayoutElems.push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
			LayoutElems.push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
			LayoutElems.push_back(LayoutElement(4, 0, 3, VT_FLOAT32, false));  //Bitangents
			LayoutElems.push_back(LayoutElement(5, 0, 4, VT_INT32, false));    //BONE ID
			LayoutElems.push_back(LayoutElement(6, 0, 4, VT_FLOAT32, false));  //WEIGHT

			auto source = Platform::FileSystem::GetInstance().LoadFileToString("@NuclearAssets@/Shaders/PositionalShadowDepthPass.hlsl");

			//Create Vertex Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "PositionalShadowMapDepthVS";
				CreationAttribs.Desc.Name = "Positional_ShadowMapDepthVS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, &VSShader);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "PositionalShadowMapDepthPS";
				CreationAttribs.Desc.Name = "Positional_ShadowMapDepthPS";
				CreationAttribs.Source = source.c_str();


				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, &PSShader);
			}
			
			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());

			Graphics::PSOResourcesInitInfo ResourcesInitinfo;
			Graphics::GraphicsEngine::GetInstance().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

			Graphics::Context::GetInstance().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mPositionalShadowMapDepthPSO);


			BufferDesc CBDesc;
			CBDesc.Name = "PositionalLightInfo_CB";
			CBDesc.Size = sizeof(NEStatic_LightInfo);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pPositionalLightInfoCB);

			mPositionalShadowMapDepthPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_LightInfo")->Set(pPositionalLightInfoCB);

			mPositionalShadowMapDepthPSO->CreateShaderResourceBinding(mPositionalShadowMapDepthSRB.RawDblPtr(), true);
		}
		void ShadowPass::InitPositionalShadowMapTextures()
		{
			if(mDesc.MAX_DIR_CASTERS > 0)
			{
				TextureDesc ShadowMapDesc;
				ShadowMapDesc.Name = "DirPosShadowMap";
				ShadowMapDesc.Width = mDesc.mDirPosShadowMapInfo.mResolution;
				ShadowMapDesc.Height = mDesc.mDirPosShadowMapInfo.mResolution;
				ShadowMapDesc.MipLevels = 1;
				ShadowMapDesc.SampleCount = 1;
				ShadowMapDesc.Format = TEX_FORMAT_R32_TYPELESS;
				ShadowMapDesc.Usage = USAGE_DEFAULT;
				ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;
				ShadowMapDesc.Type = RESOURCE_DIM_TEX_2D_ARRAY;
				ShadowMapDesc.ArraySize = mDesc.MAX_DIR_CASTERS;

				Graphics::Context::GetInstance().GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &mDirShadowMap.pPosShadowMap);

				{
					TextureViewDesc SRVDesc{ "DirPosShadowMap_SRV", TEXTURE_VIEW_SHADER_RESOURCE, RESOURCE_DIM_TEX_2D_ARRAY };
					SRVDesc.Format = TEX_FORMAT_R32_FLOAT;
					mDirShadowMap.pPosShadowMap->CreateView(SRVDesc, &mDirShadowMap.pPosShadowMapSRV);
				}

				mDirShadowMap.pPosShadowMapDSVs.clear();
				mDirShadowMap.pPosShadowMapDSVs.resize(ShadowMapDesc.ArraySize);
				for (Uint32 iArrSlice = 0; iArrSlice < ShadowMapDesc.ArraySize; iArrSlice++)   //To avoid recreating each frame
				{
					TextureViewDesc ShadowMapDSVDesc;
					ShadowMapDSVDesc.Name = "DirPosShadowMap DSV";
					ShadowMapDSVDesc.ViewType = TEXTURE_VIEW_DEPTH_STENCIL;
					ShadowMapDSVDesc.FirstArraySlice = iArrSlice;
					ShadowMapDSVDesc.NumArraySlices = 1;
					mDirShadowMap.pPosShadowMap->CreateView(ShadowMapDSVDesc, &mDirShadowMap.pPosShadowMapDSVs[iArrSlice]);
				}
			}

			if (mDesc.MAX_SPOT_CASTERS > 0)
			{
				TextureDesc ShadowMapDesc;
				ShadowMapDesc.Name = "SpotShadowMap";
				ShadowMapDesc.Width = mDesc.mSpotShadowMapInfo.mResolution;
				ShadowMapDesc.Height = mDesc.mSpotShadowMapInfo.mResolution;
				ShadowMapDesc.MipLevels = 1;
				ShadowMapDesc.SampleCount = 1;
				ShadowMapDesc.Format = TEX_FORMAT_R32_TYPELESS;
				ShadowMapDesc.Usage = USAGE_DEFAULT;
				ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;
				ShadowMapDesc.Type = RESOURCE_DIM_TEX_2D_ARRAY;
				ShadowMapDesc.ArraySize = mDesc.MAX_SPOT_CASTERS;

				Graphics::Context::GetInstance().GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &mSpotShadowMap.pPosShadowMap);

				{
					TextureViewDesc SRVDesc{ "SpotShadowMap_SRV", TEXTURE_VIEW_SHADER_RESOURCE, RESOURCE_DIM_TEX_2D_ARRAY };
					SRVDesc.Format = TEX_FORMAT_R32_FLOAT;
					mSpotShadowMap.pPosShadowMap->CreateView(SRVDesc, &mSpotShadowMap.pPosShadowMapSRV);
				}

				mSpotShadowMap.pPosShadowMapDSVs.clear();
				mSpotShadowMap.pPosShadowMapDSVs.resize(ShadowMapDesc.ArraySize);
				for (Uint32 iArrSlice = 0; iArrSlice < ShadowMapDesc.ArraySize; iArrSlice++)   //To avoid recreating each frame
				{
					TextureViewDesc ShadowMapDSVDesc;
					ShadowMapDSVDesc.Name = "SpotShadowMap DSV";
					ShadowMapDSVDesc.ViewType = TEXTURE_VIEW_DEPTH_STENCIL;
					ShadowMapDSVDesc.FirstArraySlice = iArrSlice;
					ShadowMapDSVDesc.NumArraySlices = 1;
					mSpotShadowMap.pPosShadowMap->CreateView(ShadowMapDSVDesc, &mSpotShadowMap.pPosShadowMapDSVs[iArrSlice]);
				}
			}
		}
		void ShadowPass::InitOmniDirShadowPassPSO()
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "OmniDirShadowMapDepthPass PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 0;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_D32_FLOAT;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = false;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.StencilEnable = false;
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthBias = 8500; //maybe as parameter
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthBiasClamp = 0.0f;
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.SlopeScaledDepthBias = 1.0f;
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthClipEnable = true;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> GSShader;
			RefCntAutoPtr<IShader> PSShader;
			std::vector<LayoutElement> LayoutElems;

			LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
			LayoutElems.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
			LayoutElems.push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
			LayoutElems.push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
			LayoutElems.push_back(LayoutElement(4, 0, 3, VT_FLOAT32, false));  //Bitangents
			LayoutElems.push_back(LayoutElement(5, 0, 4, VT_INT32, false));    //BONE ID
			LayoutElems.push_back(LayoutElement(6, 0, 4, VT_FLOAT32, false));  //WEIGHT

			auto source = Platform::FileSystem::GetInstance().LoadFileToString("@NuclearAssets@/Shaders/OmniDirShadowDepthPass.hlsl");

			//Create Vertex Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "OmniDirShadowMapDepthVS";
				CreationAttribs.Desc.Name = "OmniDir_ShadowMapDepthVS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, &VSShader);
			}

			//Create Geometry Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
				CreationAttribs.EntryPoint = "OmniDirShadowMapDepthGS";
				CreationAttribs.Desc.Name = "OmniDir_ShadowMapDepthGS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, &GSShader);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "OmniDirShadowMapDepthPS";
				CreationAttribs.Desc.Name = "OmniDir_ShadowMapDepthPS";
				CreationAttribs.Source = source.c_str();


				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, &PSShader);
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pGS = GSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());

			Graphics::PSOResourcesInitInfo ResourcesInitinfo;
			Graphics::GraphicsEngine::GetInstance().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

			Graphics::Context::GetInstance().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mOmniDirShadowPassPSO);

			{
				BufferDesc CBDesc;
				CBDesc.Name = "NEStatic_OmniDirShadowVS";
				CBDesc.Size = sizeof(Math::Matrix4);
				CBDesc.Usage = USAGE_DYNAMIC;
				CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
				CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
				Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pOmniDirShadowVS_CB);

				CBDesc.Name = "NEStatic_OmniDirShadowGS";
				CBDesc.Size = sizeof(Math::Matrix4) * 6;
				Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pOmniDirShadowGS_CB);

				CBDesc.Name = "NEStatic_OmniDirShadowPS";
				CBDesc.Size = sizeof(Math::Vector4);

				Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pOmniDirShadowPS_CB);
			}
			mOmniDirShadowPassPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_OmniDirShadowVS")->Set(pOmniDirShadowVS_CB);
			mOmniDirShadowPassPSO->GetStaticVariableByName(SHADER_TYPE_GEOMETRY, "NEStatic_OmniDirShadowGS")->Set(pOmniDirShadowGS_CB);
			mOmniDirShadowPassPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "NEStatic_OmniDirShadowPS")->Set(pOmniDirShadowPS_CB);

			mOmniDirShadowPassPSO->CreateShaderResourceBinding(mOmniDirShadowPassSRB.RawDblPtr(), true);
		}
		void ShadowPass::InitOmniDirShadowMapTexture()
		{
			TextureDesc ShadowMapDesc;
			ShadowMapDesc.Name = "ShadowMap";
			ShadowMapDesc.Width = mDesc.mOmniDirShadowMapInfo.mResolution;
			ShadowMapDesc.Height = mDesc.mOmniDirShadowMapInfo.mResolution;
			ShadowMapDesc.MipLevels = 1;
			ShadowMapDesc.SampleCount = 1;
			ShadowMapDesc.Format = TEX_FORMAT_R32_TYPELESS;
			ShadowMapDesc.Usage = USAGE_DEFAULT;
			ShadowMapDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;

			ShadowMapDesc.ArraySize = 6 * mDesc.MAX_OMNIDIR_CASTERS;

			ShadowMapDesc.Type = RESOURCE_DIM_TEX_CUBE_ARRAY;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(ShadowMapDesc, nullptr, &pOmniDirShadowMap);

			{
				TextureViewDesc SRVDesc{ "ShadowMap_SRV", TEXTURE_VIEW_SHADER_RESOURCE, RESOURCE_DIM_TEX_CUBE_ARRAY };
				SRVDesc.Format = TEX_FORMAT_R32_FLOAT;
				pOmniDirShadowMap->CreateView(SRVDesc, &pOmniDirShadowMapSRV);
			}
			//{
			//	TextureViewDesc DSVDesc{ "ShadowMap_DSV", TEXTURE_VIEW_DEPTH_STENCIL,  RESOURCE_DIM_TEX_2D_ARRAY };
			//	DSVDesc.Format = TEX_FORMAT_D32_FLOAT;
			//	pOmniDirShadowMap->CreateView(DSVDesc, &pOmniDirShadowMapRTV);
			//}
			pOmniDirShadowMapDSVs.clear();
			pOmniDirShadowMapDSVs.resize(mDesc.MAX_OMNIDIR_CASTERS);

			for (Uint32 i = 0, firstarrayslice = 0; i < mDesc.MAX_OMNIDIR_CASTERS; i++, firstarrayslice += 6)   //To avoid recreating each frame
			{
				TextureViewDesc ShadowMapDSVDesc;
				ShadowMapDSVDesc.Name = "OmniDIrShadowMap DSV";
				ShadowMapDSVDesc.ViewType = TEXTURE_VIEW_DEPTH_STENCIL;
				ShadowMapDSVDesc.FirstArraySlice = firstarrayslice;
				ShadowMapDSVDesc.NumArraySlices = 6;
				ShadowMapDSVDesc.Format = TEX_FORMAT_D32_FLOAT;
				ShadowMapDSVDesc.TextureDim = RESOURCE_DIM_TEX_2D_ARRAY;
				pOmniDirShadowMap->CreateView(ShadowMapDSVDesc, &pOmniDirShadowMapDSVs[i]);
			}

		}
		void ShadowPass::RenderMeshForDepthPass(Assets::Mesh* mesh)
		{
			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{

				Graphics::Context::GetInstance().GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetInstance().GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetInstance().GetContext()->DrawIndexed(DrawAttrs);

			}
		}
	}
}