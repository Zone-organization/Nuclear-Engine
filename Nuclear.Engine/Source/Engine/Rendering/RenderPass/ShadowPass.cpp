#include "Engine\Rendering\RenderPass\ShadowPass.h"
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Assets/Mesh.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine\Components\EntityInfoComponent.h>
#include <Engine\Assets\Scene.h>

namespace Nuclear
{
	namespace Rendering
	{
		struct NEStatic_LightInfo {
			Math::Matrix4 Model;
			Math::Matrix4 LightSpace;
		};

		ShadowPass::ShadowPass(const ShadowPassDesc& desc)
		{
			mDesc = desc;
		}

		void ShadowPass::Initialize()
		{
			InitDirLightSimpleShadowPassPSO();
			InitSpotLightShadowPSO();
			InitPointLightShadowPassPSO();

			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_ShadowCasters";
			CBDesc.Size = (sizeof(Math::Matrix4) * mDesc.MAX_DIR_CASTERS) + (sizeof(Math::Matrix4) * mDesc.MAX_SPOT_CASTERS);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pVSShadowCasterBuffer);
		}

		void ShadowPass::DirLightShadowDepthPass(Components::DirLightComponent& light, Assets::Scene* scene)
		{
			//if (light.GetShadowType() == Components::LightShadowType::Simple_Shadows)
			{
				Graphics::Context::GetContext()->SetPipelineState(mDirShadowMapDepthPSO.RawPtr());

				Graphics::Context::GetContext()->SetRenderTargets(0, nullptr, light.GetShadowMap()->GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->ClearDepthStencil(light.GetShadowMap()->GetRTV(), CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Graphics::Context::GetContext()->CommitShaderResources(mDirShadowMapDepthSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				auto view = scene->GetRegistry().view<Components::MeshComponent>();
				for (auto entity : view)
				{
					auto& MeshObject = view.get<Components::MeshComponent>(entity);
					if (MeshObject.mRender && MeshObject.mCastShadows)               //TODO Animation component
					{
						auto& EntityInfo = scene->GetRegistry().get<Components::EntityInfoComponent>(entity);
						EntityInfo.mTransform.Update();

						NEStatic_LightInfo cbdata;
						cbdata.Model = EntityInfo.mTransform.GetWorldMatrix();
						cbdata.LightSpace = light.LightSpace;
						{
							Diligent::MapHelper<NEStatic_LightInfo> CBConstants(Graphics::Context::GetContext(), pDirLightInfoCB, MAP_WRITE, MAP_FLAG_DISCARD);
							*CBConstants = cbdata;
						}

						RenderMeshForDepthPass(MeshObject.mMesh);
					}
				}
			}
		}

		void ShadowPass::SpotLightShadowDepthPass(Components::SpotLightComponent& spotlight, Assets::Scene* scene)
		{
			//float near_plane = 1.0f, far_plane = 100.f;
			//auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

			//auto lightpos = spotlight.GetInternalPosition();
			// Also re-calculate the Right and Up vector
		//	auto Right = normalize(Math::cross(spotlight.GetDirection(), glm::vec3(0.0, 1.0, 0.0)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			//auto Up = normalize(Math::cross(Right, spotlight.GetDirection()));

			//auto lightView = glm::lookAt(lightpos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
		/*	auto up = glm::vec3(0.0, 1.0, 0.0);


			auto target = lightpos + glm::vec3(0.0f);


			auto lightView = glm::lookAt(lightpos, spotlight.GetDirection(), up);
			Math::Matrix4 lightSpaceMatrix = lightProjection * lightView;*/

			Graphics::Context::GetContext()->SetPipelineState(mSpotShadowMapDepthPSO.RawPtr());

			Graphics::Context::GetContext()->SetRenderTargets(0, nullptr, spotlight.GetShadowMap()->GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(spotlight.GetShadowMap()->GetRTV(), CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetContext()->CommitShaderResources(mSpotShadowMapDepthSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			auto view = scene->GetRegistry().view<Components::MeshComponent>();
			for (auto entity : view)
			{
				auto& MeshObject = view.get<Components::MeshComponent>(entity);
				if (MeshObject.mRender && MeshObject.mCastShadows)               //TODO Animation component
				{
					auto& EntityInfo = scene->GetRegistry().get<Components::EntityInfoComponent>(entity);
					EntityInfo.mTransform.Update();

					NEStatic_LightInfo cbdata;
					cbdata.Model = EntityInfo.mTransform.GetWorldMatrix();
					cbdata.LightSpace = spotlight.LightSpace;
					{
						Diligent::MapHelper<NEStatic_LightInfo> CBConstants(Graphics::Context::GetContext(), pSpotLightInfoCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = cbdata;
					}

					RenderMeshForDepthPass(MeshObject.mMesh);
				}
			}
		}

		void ShadowPass::PointLightShadowDepthPass(Components::PointLightComponent& pointlight, Assets::Scene* scene)
		{
		}

		ShadowPassDesc ShadowPass::GetDesc() const
		{
			return mDesc;
		}

		IBuffer* ShadowPass::GetShadowCastersCB()
		{
			return pVSShadowCasterBuffer;
		}

		void ShadowPass::InitDirLightSimpleShadowPassPSO()
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "mDirShadowMapDepthPSO";
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

			auto source = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/DirShadowDepthPass.hlsl");

			//Create Vertex Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "DirShadowMapDepthVS";
				CreationAttribs.Desc.Name = "Dir_ShadowMapDepthVS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VSShader);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "DirShadowMapDepthPS";
				CreationAttribs.Desc.Name = "Dir_ShadowMapDepthPS";
				CreationAttribs.Source = source.c_str();


				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mDirShadowMapDepthPSO, PSOCreateInfo, Vars, true);

			BufferDesc CBDesc;
			CBDesc.Name = "DirLightInfo_CB";
			CBDesc.Size = sizeof(NEStatic_LightInfo);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pDirLightInfoCB);

			mDirShadowMapDepthPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_LightInfo")->Set(pDirLightInfoCB);

			mDirShadowMapDepthPSO->CreateShaderResourceBinding(mDirShadowMapDepthSRB.RawDblPtr(), true);
		}

		void ShadowPass::InitSpotLightShadowPSO()
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "mSpotShadowMapDepthPSO";
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

			auto source = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/SpotShadowDepthPass.hlsl");

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
		void ShadowPass::InitPointLightShadowPassPSO()
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "PointShadowPassPSO";
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

			auto source = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/PointShadowDepthPass.hlsl");

			//Create Vertex Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "PointShadowMapDepthVS";
				CreationAttribs.Desc.Name = "Point_ShadowMapDepthVS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VSShader);
			}

			//Create Geometry Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
				CreationAttribs.EntryPoint = "PointShadowMapDepthGS";
				CreationAttribs.Desc.Name = "Point_ShadowMapDepthGS";

				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &GSShader);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "PointShadowMapDepthPS";
				CreationAttribs.Desc.Name = "Point_ShadowMapDepthPS";
				CreationAttribs.Source = source.c_str();


				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pGS = GSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPointShadowPassPSO, PSOCreateInfo, Vars, true);

			{
				BufferDesc CBDesc;
				CBDesc.Name = "NEStatic_PointShadowVS";
				CBDesc.Size = sizeof(Math::Matrix4);
				CBDesc.Usage = USAGE_DYNAMIC;
				CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
				CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
				Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pPointShadowVS_CB);

				CBDesc.Name = "NEStatic_PointShadowGS";
				CBDesc.Size = sizeof(Math::Matrix4) * 6;
				Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pPointShadowGS_CB);

				CBDesc.Name = "NEStatic_PointShadowPS";
				CBDesc.Size = sizeof(Math::Vector4);

				Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pPointShadowPS_CB);
			}
			mPointShadowPassPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_PointShadowVS")->Set(pPointShadowVS_CB);
			mPointShadowPassPSO->GetStaticVariableByName(SHADER_TYPE_GEOMETRY, "NEStatic_PointShadowGS")->Set(pPointShadowGS_CB);
			mPointShadowPassPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "NEStatic_PointShadowPS")->Set(pPointShadowPS_CB);

			mPointShadowPassPSO->CreateShaderResourceBinding(mPointShadowPassSRB.RawDblPtr(), true);
		}
		void ShadowPass::RenderMeshForDepthPass(Assets::Mesh* mesh)
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
	}
}