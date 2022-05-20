#include <Engine\Systems\LightingSystem.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\ECS\Scene.h>
#include <Engine\Components\EntityInfoComponent.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Components/MeshComponent.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\FileSystem.h>

namespace Nuclear
{
	namespace Systems
	{

		LightingSystem::LightingSystem(const LightingSystemDesc& desc)
		{
			mDesc = desc;
		}
		bool LightingSystem::RequiresBaking()
		{
			if (Baked_DirLights_Size == DirLights.size() && Baked_PointLights_Size == PointLights.size() && Baked_SpotLights_Size == SpotLights.size())
				return true;

			return false;
		}
		IBuffer* LightingSystem::GetLightCB()
		{
			return mPSLightCB.RawPtr();
		}
		size_t LightingSystem::GetDirLightsNum()
		{
			return DirLights.size();
		}
		size_t LightingSystem::GetPointLightsNum()
		{
			return PointLights.size();
		}
		size_t LightingSystem::GetSpotLightsNum()
		{
			return SpotLights.size();
		}
		LightingSystemDesc LightingSystem::GetDesc() const
		{
			return mDesc;
		}
		void LightingSystem::BakeBuffer()
		{
			if (HasbeenBakedBefore)
			{
				if (!RequiresBaking())
				{
					NUCLEAR_WARN("[LightingSystem] No need for baking the sub system!");
				}
			}

			HasbeenBakedBefore = true;

			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
			Baked_DirLights_Size = DirLights.size();
			Baked_PointLights_Size = PointLights.size();
			Baked_SpotLights_Size = SpotLights.size();

			if (DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (DirLights.size() * 2);
			}
			if (PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (PointLights.size() * 3);
			}
			if (SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (SpotLights.size() * 5);
			}

			if (mPSLightCB.RawPtr() != nullptr)
			{
				mPSLightCB.Release();
			}
			BufferDesc CBDesc;
			CBDesc.Name = "LightCB";
			CBDesc.Size = static_cast<Uint32>(NE_Light_CB_Size);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			BufferData DATA;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, &DATA, mPSLightCB.RawDblPtr());
		}

		void LightingSystem::Bake()
		{
			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);

				DirLights.push_back(&DirLight);
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				if (!mDesc.DisableShadows)
				{
					if (!SpotLight.GetShadowMap()->isInitialized())
					{
						Graphics::ShadowMapDesc desc;
						desc.mResolution = mDesc.mSpotLightShadowMapInfo.mResolution;
						SpotLight.GetShadowMap()->Initialize(desc);
					}
				}
				SpotLights.push_back(&SpotLight);
			}

			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
				PointLights.push_back(&PointLight);
			}

			BakeBuffer();

			if (!mDesc.DisableShadows)
			{
				InitSpotLightShadowPSO();
			}
		}
		void LightingSystem::Update(ECS::TimeDelta dt)
		{
			//TODO: Multiple Cameras
			//Update Lights Positions
			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
			}

			if (!mDesc.DisableShadows)
			{
				auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
				for (auto entity : SpotLightView)
				{
					auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
					auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
					SpotLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

					if (SpotLight.mCastShadows)
					{
						SpotLightShadowDepthPass(SpotLight);
					}
				}
			}
		}
		void LightingSystem::UpdateBuffer(const Math::Vector4& CameraPos)
		{
			std::vector<Math::Vector4> LightsBuffer;
			LightsBuffer.reserve(NUM_OF_LIGHT_VECS + 1);

			LightsBuffer.push_back(CameraPos);

			for (size_t i = 0; i < DirLights.size(); i++)
			{
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Color);
			}
			for (size_t i = 0; i < PointLights.size(); i++)
			{
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Color);

			}
			for (size_t i = 0; i < SpotLights.size(); i++)
			{
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().InnerCutOf_OuterCutoff);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Color);
			}

			PVoid data;
			Graphics::Context::GetContext()->MapBuffer(mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
			data = memcpy(data, LightsBuffer.data(), NE_Light_CB_Size);
			Graphics::Context::GetContext()->UnmapBuffer(mPSLightCB, MAP_WRITE);
		}

		struct NEStatic_LightInfo {
			Math::Matrix4 Model;
			Math::Matrix4 LightSpace;
		};

		void LightingSystem::SpotLightShadowDepthPass(Components::SpotLightComponent& spotlight)
		{
			float near_plane = 1.0f, far_plane = 100.f;
			auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

			auto lightpos = spotlight.GetInternalPosition();
			// Also re-calculate the Right and Up vector
		//	auto Right = normalize(Math::cross(spotlight.GetDirection(), glm::vec3(0.0, 1.0, 0.0)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			//auto Up = normalize(Math::cross(Right, spotlight.GetDirection()));

			auto lightView = glm::lookAt(lightpos, glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(0.0, 1.0, 0.0));
		//	auto up = glm::vec3(0.0, 1.0, 0.0);


			//auto target = lightpos + glm::vec3(0.0f);


			//auto lightView = glm::lookAt(lightpos, target, up);
			Math::Matrix4 lightSpaceMatrix = lightProjection * lightView;

			Graphics::Context::GetContext()->SetPipelineState(mSpotShadowMapDepthPSO.RawPtr());

			Graphics::Context::GetContext()->SetRenderTargets(0, nullptr, spotlight.GetShadowMap()->GetDSV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(spotlight.GetShadowMap()->GetDSV(), CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetContext()->CommitShaderResources(mSpotShadowMapDepthSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			auto view = mScene->GetRegistry().view<Components::MeshComponent>();
			for (auto entity : view)
			{
				auto& MeshObject = view.get<Components::MeshComponent>(entity);
				if (MeshObject.mRender && MeshObject.mCastShadows)               //TODO Animation component
				{
					auto& EntityInfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
					EntityInfo.mTransform.Update();

					NEStatic_LightInfo cbdata;
					cbdata.Model = EntityInfo.mTransform.GetWorldMatrix();
					cbdata.LightSpace = lightSpaceMatrix;
					{
						Diligent::MapHelper<NEStatic_LightInfo> CBConstants(Graphics::Context::GetContext(), pSpotLightInfoCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = cbdata;
					}

					RenderMeshForDepthPass(MeshObject.mMesh);
				}
			}
		}
		void LightingSystem::RenderMeshForDepthPass(Assets::Mesh* mesh)
		{
			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);

			}
		}
		void LightingSystem::InitSpotLightShadowPSO()
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "mSpotShadowMapDepthPSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 0;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_R24G8_TYPELESS;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthBias = 8500; //maybe as parameter
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthBiasClamp = 0.0f;
			//PSOCreateInfo.GraphicsPipeline.RasterizerDesc.SlopeScaledDepthBias = 1.0f;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.DepthClipEnable = false;

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

			auto source = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/ShadowDepthPass.hlsl");

			//Create Vertex Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "SpotShadowMapDepthVS";
				CreationAttribs.Desc.Name = "Spot_ShadowMapDepthVS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VSShader);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "SpotShadowMapDepthPS";
				CreationAttribs.Desc.Name = "Spot_ShadowMapDepthPS";
				CreationAttribs.Source = source.c_str();


				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mSpotShadowMapDepthPSO, PSOCreateInfo, Vars, true);

			BufferDesc CBDesc;
			CBDesc.Name = "SpotLightInfo_CB";
			CBDesc.Size = sizeof(NEStatic_LightInfo);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pSpotLightInfoCB);

			mSpotShadowMapDepthPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_LightInfo")->Set(pSpotLightInfoCB);

			mSpotShadowMapDepthPSO->CreateShaderResourceBinding(mSpotShadowMapDepthSRB.RawDblPtr(), true);
		}
	}
}