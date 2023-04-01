#include <Systems\DebugSystem.h>
#include <Graphics/ImGui.h>
#include <Components/LightComponent.h>
#include <Components/EntityInfoComponent.h>
#include <Systems/RenderSystem.h>
#include <Fallbacks/FallbacksModule.h>
#include <Core/Scene.h>
#include <Utilities/Logger.h>
#include <Assets\DefaultMeshes.h>
#include <Graphics/GraphicsModule.h>
#include <Rendering/RenderingModule.h>
#include <Assets\Texture.h>

namespace Nuclear
{
	namespace Systems
	{
		using namespace Diligent;

		DebugSystem::DebugSystem()
		{
			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;

			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "DebugSystem PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = !COORDSYSTEM_LH_ENABLED;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

			//Create Vertex Shader
			{
				std::string mVShader =

					"struct VertexInputType\
			{\
				float4 Position : ATTRIB0;\
				float2 TexCoord : ATTRIB1;\
				float3 Normals : ATTRIB2;\
				float3 Tangents : ATTRIB3;\
				float3 Bitangents : ATTRIB4;\
				int4 BoneIDs : ATTRIB5;\
				float4 Weights : ATTRIB6;\
			};\
			cbuffer NEStatic_Camera : register(b0)\
			{\
				matrix Model;\
				matrix ModelInvTranspose;\
				matrix ModelViewProjection;\
				matrix View;\
				matrix Projection;\
			};\
			cbuffer NEStatic_Animation : register(b1)\
			{\
				matrix BoneTransforms[100];\
			};\
			struct PixelInputType \
			{float4 Position : SV_POSITION; \
				float2 TexCoord : TEXCOORD0; \
			}; \
				PixelInputType main(VertexInputType input)\
			{\
				PixelInputType output;\
				float4 FinalPos = float4(input.Position.xyz, 1.0f);\
				for (int i = 0; i < 4; i++)\
				{\
					if (input.BoneIDs[i] == -1)\
					{\
						continue;\
					}\
					if (input.BoneIDs[i] >= 100)\
					{\
						break;\
					}\
					FinalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);\
					float4 localPosition = mul(BoneTransforms[input.BoneIDs[i]], float4(input.Position.xyz, 1.0f));\
					FinalPos += mul(input.Weights[i], float4(localPosition.xyz, 1.0f));\
				}\
				output.Position = mul(ModelViewProjection, FinalPos);\
				output.TexCoord = input.TexCoord;\
				return output;\
			}";

				ShaderCreateInfo CreationAttribs;

				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "DebugSystemVS";

				CreationAttribs.Source = mVShader.c_str();
				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, VShader.RawDblPtr());
			}

			//Create Pixel Shader
			{
				std::string mPShader =
					"struct PixelInputType \
			{\
				float4 Position : SV_POSITION;\
				float2 UV : TEXCOORD0;\
			};\
			Texture2D NEMat_Diffuse1 : register(t0);\
			SamplerState NEMat_Diffuse1_sampler : register(s0);\
			float4 main(PixelInputType input) : SV_TARGET\
			{\
				float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);\
				result = NEMat_Diffuse1.Sample(NEMat_Diffuse1_sampler, input.UV);\
				return result;\
			}";

				ShaderCreateInfo CreationAttribs;

				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "DebugSystemPS";

				CreationAttribs.Source = mPShader.c_str();
				Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, PShader.RawDblPtr());
			}
			PSOCreateInfo.pVS = VShader;
			PSOCreateInfo.pPS = PShader;

			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Graphics::GraphicsModule::GetInstance().GetRendering3DInputLayout().data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Graphics::GraphicsModule::GetInstance().GetRendering3DInputLayout().size());

			Graphics::PSOResourcesInitInfo ResourcesInitinfo;
			Graphics::GraphicsModule::GetInstance().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

			Graphics::Context::GetInstance().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &pShader.mPipeline);

			pShader.mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(Rendering::RenderingModule::GetInstance().GetCameraCB());
			pShader.mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Animation")->Set(Rendering::RenderingModule::GetInstance().GetAnimationCB());

			pShader.mPipeline->CreateShaderResourceBinding(&pShader.mPipelineSRB, true);




			////////////////////////////////////////////	
			/*		Rendering::ShadingModelBakingDesc info;
					info.CameraBufferPtr = Core::Scene::GetInstance().GetSystemManager().GetSystem<CameraSystem>()->GetCameraCB();
					info.AnimationBufferPtr = _AnimationBufferPtr;

					DebugRP.Bake(info);
					mPipelineSRB = DebugRP.GetActiveSRB();*/
		}
		DebugSystem::~DebugSystem()
		{
		}

		void DebugSystem::Update(ECS::TimeDelta dt)
		{
			if (ShowRegisteredRenderTargets)
			{
				ShowRendertargets();
			}

			mRegisteredRTs.clear();

			//Render Light Sources
			if (RenderLightSources)
			{
				Graphics::Context::GetInstance().GetContext()->SetPipelineState(pShader.mPipeline);
				auto RTV = Graphics::Context::GetInstance().GetSwapChain()->GetCurrentBackBufferRTV();
				Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, &RTV, Rendering::RenderingModule::GetInstance().GetFinalDepthRT().GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				
				{
					auto view = Core::Scene::GetInstance().GetRegistry().view<Components::LightComponent>();
					for (auto entity : view)
					{
						auto& LightComp = view.get<Components::LightComponent>(entity);
						auto& EntityInfo = Core::Scene::GetInstance().GetRegistry().get<Components::EntityInfoComponent>(entity);

						EntityInfo.mTransform.Update();
						Core::Scene::GetInstance().GetMainCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
						Rendering::RenderingModule::GetInstance().UpdateCameraCB(Core::Scene::GetInstance().GetMainCamera());

						auto AnimationBufferPtr = Rendering::RenderingModule::GetInstance().GetAnimationCB();

						Math::Matrix4 empty(0.0f);
						PVoid data;
						Graphics::Context::GetInstance().GetContext()->MapBuffer(AnimationBufferPtr, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
						data = memcpy(data, &empty, sizeof(Math::Matrix4));
						Graphics::Context::GetInstance().GetContext()->UnmapBuffer(AnimationBufferPtr, MAP_WRITE);


						InstantRender(Assets::DefaultMeshes::GetSphereAsset(),Fallbacks::FallbacksModule::GetInstance().GetDefaultGreyImage());


						//TODO: Render Cube at direction
						/*EntityInfo.mTransform.Update();
						Core::Scene::GetInstance().GetSystemManager().GetSystem<CameraSystem>()->GetMainCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
						Core::Scene::GetInstance().GetSystemManager().GetSystem<CameraSystem>()->UpdateBuffer();

						InstantRender(Assets::DefaultMeshes::GetCubeAsset(), Assets::AssetManager::DefaultGreyTex.GetImage());*/
					}
				}
			}

		}
		void DebugSystem::ShowRendertargets()
		{
			ImGui::Begin("DebugSystem - Registered RenderTargets");

			std::vector<const char*> rt_gr;

			std::set<std::string> RT_Groups;
			for (auto& i : mRegisteredRTs)
			{
				RT_Groups.insert(i->GetDesc().mType);
			}
			for (auto& i : RT_Groups)
			{
				rt_gr.push_back(i.c_str());
			}

			static int item_current = 0;
			ImGui::Combo("RenderTarget Type", &item_current, rt_gr.data(), rt_gr.size());

			for (Uint32 i = 0; i < mRegisteredRTs.size(); i++)
			{
				auto& ref = mRegisteredRTs.at(i);
				if (ref->GetDesc().mType == rt_gr.at(item_current))
				{
					ImGui::Text(ref->GetDesc().mName.c_str());
					ImGui::SameLine();
					ImGui::Image(ref->GetSRV(), { 256.f,256.f });
					if (i % 2 == 0)
					{
						ImGui::SameLine();
					}
				}
			}

			ImGui::End();

		}
		void DebugSystem::AddRenderTarget(Graphics::RenderTarget* rt)
		{
			mRegisteredRTs.push_back(rt);
		}
		void DebugSystem::InstantRender(Assets::Mesh* mesh, Assets::Texture* diffusetex)
		{

			if (mesh == nullptr)
			{
				NUCLEAR_ERROR("[DebugSystem] Skipped Rendering invalid Mesh...");
				return;
			}
			if (diffusetex == nullptr)
			{
				NUCLEAR_ERROR("[DebugSystem] Skipped Rendering Mesh with invalid image...");
				return;
			}


		Uint64 offset = 0;

		pShader.mPipelineSRB->GetVariableByName(SHADER_TYPE_PIXEL, "NEMat_Diffuse1")->Set(diffusetex->GetTextureView());
		Graphics::Context::GetInstance().GetContext()->CommitShaderResources(pShader.mPipelineSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

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