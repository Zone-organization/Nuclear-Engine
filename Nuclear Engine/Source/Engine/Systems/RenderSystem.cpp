#include <Engine\Systems\RenderSystem.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/TransformComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Assets\Material.h>
#include "Diligent\Graphics\GraphicsEngine\interface\MapHelper.h"

namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(const RenderSystemDesc & desc, Components::CameraComponent* camera)
		{
			Desc = desc;
			ActiveCamera = camera;
			this->status = RenderSystemStatus::RequireBaking;
		}
		RenderSystem::~RenderSystem()
		{
		}
		void RenderSystem::BakePipeline()
		{
			// Pipeline state object encompasses configuration of all GPU stages

			PipelineStateDesc PSODesc;

			PSODesc.Name = "RenderSystem PSO";
			// This is a graphics pipeline
			PSODesc.IsComputePipeline = false;
			// This tutorial will render to a single render target
			PSODesc.GraphicsPipeline.NumRenderTargets = 1;
			// Set render target format which is the format of the swap chain's color buffer
			PSODesc.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSODesc.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			//PSODesc.GraphicsPipeline.BlendDesc.RenderTargets[0].

			// Set depth buffer format which is the format of the swap chain's back buffer
			PSODesc.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			// Primitive topology defines what kind of primitives will be rendered by this pipeline state
			PSODesc.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			// Cull back faces
			PSODesc.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			// Enable depth testing
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;


			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			std::vector<LayoutElement> LayoutElems;

			//Create Vertex Shader
			{
				Managers::AutoVertexShaderDesc VertShaderDesc;
				//if (Desc.NormalMaps == true) { VertShaderDesc.InTangents = true; }

				VSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoVertexShader(VertShaderDesc, &LayoutElems);
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "RenderSystem PixelShader";
				

				std::vector<std::string> defines;

				if (this->DirLights.size() > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(DirLights.size())); }
				if (this->PointLights.size() > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(PointLights.size())); }
				if (this->SpotLights.size() > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(SpotLights.size())); }
				if (Desc.NormalMaps) { defines.push_back("NE_USE_NORMAL_MAPS"); }

				auto source = Core::FileSystem::LoadShader(Desc.PShaderPath, defines, std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
				Calculate_Light_CB_Size();
			}
			PSODesc.GraphicsPipeline.pVS = VSShader;
			PSODesc.GraphicsPipeline.pPS = PSShader;
			PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSODesc.GraphicsPipeline.InputLayout.NumElements = LayoutElems.size();
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);


			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPipeline, PSODesc, Vars, true);

			//Set pipeline static vars
			mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(ActiveCamera->GetCBuffer());
			mPipeline->GetStaticVariableByName(SHADER_TYPE_PIXEL, "NEStatic_Lights")->Set(mPSLightCB);

		}
		void RenderSystem::InitializePostProcessing(unsigned int WindowWidth, unsigned int WindowHeight)
		{
			/*ITextureDescriptor ScreenTexDesc;
			if (Desc.HDR == true)
			{
				ScreenTexDesc.format = IFormat::RGBA16Float;
			}
			else
			{
				ScreenTexDesc.format = IFormat::RGBA8UNorm;
			}
			ScreenTexDesc.type = ITextureType::Texture2D;

			ISrcImageDescriptor Data;
			Data.data = NULL;
			ScreenTexDesc.extent.width = WindowWidth;
			ScreenTexDesc.extent.height = WindowHeight;

			PostProcessTexture = IContext::GetRenderer()->CreateTexture(ScreenTexDesc, &Data);

			IRenderTargetDescriptor RTDesc;
			RTDesc.resolution.width = WindowWidth;
			RTDesc.resolution.height = WindowHeight;
			RTDesc.attachments.push_back({ IAttachmentType::Color, PostProcessTexture });
			RTDesc.attachments.push_back({ IAttachmentType::DepthStencil });

			PostProcessRT = IContext::GetRenderer()->CreateRenderTarget(RTDesc);

			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Use_Camera = false;
			VertShaderDesc.InNormals = false;
			VertShaderDesc.OutFragPos = false;

			PostProcess_VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
			Assets::Mesh::CreateScreenQuad(&PostProcessScreenQuad);
			PostProcessScreenQuad.Initialize(PostProcess_VShader);
			std::vector<std::string> defines;
			if (Desc.GammaCorrection == true) { defines.push_back("NE_GAMMA_CORRECTION_ENABLED"); }
			if (Desc.HDR == true) { defines.push_back("NE_HDR_ENABLED"); }


			IShaderCompiler::CompileAndCreateShader(&PostProcess_PShader,
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
				IShaderType::Fragment);
	
			ISamplerDescriptor Samplerdesc;
			Samplerdesc.magFilter = Samplerdesc.minFilter = Samplerdesc.mipMapFilter = ISamplerFilter::Nearest;
			ScreenSampler = IContext::GetRenderer()->CreateSampler(Samplerdesc);*/
		}
		void RenderSystem::SetCamera(Components::CameraComponent * camera)
		{
			this->ActiveCamera = camera;
		}
		Components::CameraComponent* RenderSystem::GetCamera()
		{
			return this->ActiveCamera;
		}

		void RenderSystem::AddLight(Components::DirectionalLight * light)
		{
			DirLights.push_back(light);
		}
		void RenderSystem::AddLight(Components::PointLight * light)
		{
			PointLights.push_back(light);
		}
		void RenderSystem::AddLight(Components::SpotLight * light)
		{
			SpotLights.push_back(light);
		}

		void RenderSystem::CreateMaterial(Assets::Material * material)
		{
			if (!material)
				return;

			Assets::MaterialCreationDesc desc;
			desc.mPipeline = mPipeline.RawPtr();
			material->Create(desc);
		}

		void RenderSystem::Bake()
		{
			BakeLightConstantBuffer();
			BakePipeline();
			Update_Light();
		}
		IPipelineState * RenderSystem::GetPipeline()
		{
			return mPipeline.RawPtr();
		}

		void RenderSystem::BakeLightConstantBuffer()
		{	
			BufferDesc CBDesc;
			CBDesc.Name = "RenderSystem LightCB";
			CBDesc.uiSizeInBytes = NE_Light_CB_Size;
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, &BufferData(), mPSLightCB.GetRawDblPtr());
		}

		void RenderSystem::Update_Light()
		{
			std::vector<Math::Vector4> LightsBuffer;
			//LightsBuffer.reserve(NUM_OF_LIGHT_VECS);

			LightsBuffer.push_back(Math::Vector4(ActiveCamera->GetPosition(), 1.0f));

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

			//Diligent::MapHelper<Math::Vector4> CBConstants(Graphics::Context::GetContext(), mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD);
			//*CBConstants = *LightsBuffer.data();


			Math::Vector4* data;
			Graphics::Context::GetContext()->MapBuffer(mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
			data = LightsBuffer.data();
			Graphics::Context::GetContext()->UnmapBuffer(mPSLightCB, MAP_WRITE);
		}

		void RenderSystem::Update_Meshes(ECS::EntityManager & es)
		{
			ECS::ComponentHandle<Components::MeshComponent> MeshObject;
			for (ECS::Entity entity : es.entities_with_components(MeshObject))
			{
				if (!MeshObject.Get()->mMultiRender)
				{
					//entity.GetTransform()->Update();
					ActiveCamera->SetModelMatrix(entity.GetComponent<Components::TransformComponent>().Get()->GetTransformMatrix());
					InstantRender(MeshObject.Get());
				}
				else 
				{
					for (auto i : MeshObject.Get()->mMultiRenderTransforms)
					{
						ActiveCamera->SetModelMatrix(i);
						InstantRender(MeshObject.Get());
					}
				}
			}
		}
		void RenderSystem::Update(ECS::EntityManager & es, ECS::EventManager & events, ECS::TimeDelta dt)
		{
			Update_Meshes(es);
			Update_Light();
		}
		void RenderSystem::InstantRender(Components::MeshComponent * object)
		{
			if (object == nullptr)
			{
				Log.Error("[RenderSystem] Skipped Rendering invalid MeshComponent...\n");
				return;
			}
			if (object->mMesh == nullptr)
			{
				Log.Error("[RenderSystem] Skipped Rendering invalid Mesh...\n");
				return;
			}
			if (object->mMaterial == nullptr)
			{
				Log.Error("[RenderSystem] Skipped Rendering Mesh with invalid Material...\n");
				return;
			}

			InstantRender(object->mMesh, object->mMaterial);
		}
		void RenderSystem::InstantRender(Assets::Mesh * mesh, Assets::Material* material)
		{
			material->Bind();
			Uint32 offset = 0;

			for (size_t i = 0; i< mesh->mSubMeshes.size(); i++)
			{
				material->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
				
				DrawAttribs DrawAttrs;
				DrawAttrs.IsIndexed = true;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->Draw(DrawAttrs);
			}
		}
		void RenderSystem::RenderToPostProcessingRT()
		{
			///PostProcessRT.Bind();
			///IContext::Clear(IColor(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		}
		void RenderSystem::RenderPostProcessingContents()
		{
			///PostProcessRT.Bind_Default();
			///IContext::Clear(IColor(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
			///PostProcess_VShader.Bind();
			///PostProcess_PShader.Bind();
			///ScreenSampler.PSBind(0);
			///PostProcessTexture.PSBind(0);
			///InstantRender(&PostProcessScreenQuad, nullptr);
		}
	
		void RenderSystem::Calculate_Light_CB_Size()
		{
			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
			if (this->DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (this->DirLights.size() * 2);
			}
			if (this->PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (this->PointLights.size() * 3);
			}
			if (this->SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (this->SpotLights.size() * 5);
			}
		}

	}
}